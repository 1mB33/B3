#include <cassert>
#include <chrono>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <memory>
#include <ostream>
#include <string_view>
#include <unordered_map>

using namespace std;

#define B33_ASSET( className )                                                                                         \
  public:                                                                                                              \
    static string_view GetAssetTypeName()                                                                              \
    {                                                                                                                  \
        return #className;                                                                                             \
    }                                                                                                                  \
                                                                                                                       \
  private:

// --------------------------------------------------------------------------------------------------------------------
class Mesh
{
    B33_ASSET( Mesh );

  public:
    Mesh()  = default;
    ~Mesh() = default;

    void Action() const
    {
        cout << "I'm a mesh" << endl;
    }
};

// --------------------------------------------------------------------------------------------------------------------
class Texture
{
    B33_ASSET( Texture );

  public:
    Texture()  = default;
    ~Texture() = default;
};

// --------------------------------------------------------------------------------------------------------------------
class Sound
{
    B33_ASSET( Sound );

  public:
    Sound()  = default;
    ~Sound() = default;
};

// --------------------------------------------------------------------------------------------------------------------
enum EAssetType
{
    EUnkonwn,
    EMesh,
    ETexture,
    ESound,
};

// --------------------------------------------------------------------------------------------------------------------
struct AssetMetadata
{
    using Clock = chrono::system_clock;

    bool              Valid;
    EAssetType        Type;
    Clock::time_point CreationTime;
    uint64_t          AssetLength;
};

// --------------------------------------------------------------------------------------------------------------------
class AssetWrapper
{
  public:
    AssetWrapper( AssetMetadata metadata, string strName )
      : m_Metadata( metadata )
      , m_strName( strName )
    {
    }

  public:
    const AssetMetadata &GetMetadata() const
    {
        return m_Metadata;
    }

    const string &GetName() const
    {
        return m_strName;
    }

  private:
    const AssetMetadata m_Metadata = {};
    const string        m_strName  = "Unknown";
};

// --------------------------------------------------------------------------------------------------------------------
template <class ASSET_TYPE>
class Asset : public AssetWrapper
{
    using Clock = chrono::system_clock;

  public:
    Asset( AssetMetadata metadata = { .Type = EAssetType::EUnkonwn, .CreationTime = Clock::now() },
           string        strName  = "UnknownAsset" )
      : AssetWrapper( metadata, strName )
      , m_Data()
    {
    }

    ~Asset() = default;

    Asset( const Asset & ) = delete;
    Asset( Asset && )      = default;

  public:
    const ASSET_TYPE &GetData() const
    {
        return m_Data;
    }

  private:
    ASSET_TYPE m_Data = {};
};

// --------------------------------------------------------------------------------------------------------------------
class AssetsReader
{
  public:
    AssetsReader()  = delete;
    ~AssetsReader() = delete;

  public:
    static void LoadAsset( string strPath, AssetMetadata &outMetadata, void **ppData )
    {
        using strm = ifstream;

        char pBuffer[ 8 ] = { 0 };

        strm file( strPath, ios_base::binary );
        if ( !file.is_open() || file.bad() )
        {
            cerr << "File is bad: " << strPath << std::endl;
            throw std::exception();
        }

        file.read( pBuffer, 8 );
        if ( pBuffer[ 0 ] != 0 && pBuffer[ 1 ] != 'B' && pBuffer[ 2 ] != '3' && pBuffer[ 3 ] != '3' )
        {
            cerr << "File doesn't start with '0B33'" << std::endl;
            throw std::exception();
        }
        outMetadata.Valid = true;

        file.read( pBuffer, 8 );
        uint64_t uCastedTime     = *reinterpret_cast<uint64_t *>( &pBuffer );
        outMetadata.CreationTime = AssetMetadata::Clock::time_point { chrono::seconds { uCastedTime } };

        file.read( pBuffer, 8 );
        outMetadata.Type = InterprateBytesToAssetType( pBuffer );

        assert( outMetadata.Valid == true );
    }

  private:
    static EAssetType InterprateBytesToAssetType( char pBytes[ 8 ] )
    {
        string strBytes( pBytes, 4 );

        if ( strBytes == "mesh" )
        {
            return EAssetType::EMesh;
        }
        if ( strBytes == "text" )
        {
            return EAssetType::ETexture;
        }
        if ( strBytes == "soun" )
        {
            return EAssetType::ESound;
        }

        cerr << "Bytes are bad: " << strBytes << std::endl;
        throw std::exception();
    }
};

// --------------------------------------------------------------------------------------------------------------------
class AssetsManager
{
  public:
    AssetsManager() = default;

    ~AssetsManager() = default;

  public:
    template <class ASSET_TYPE>
    weak_ptr<Asset<ASSET_TYPE>> GenerateAsset( string_view assetName )
    {
        cout << "Generating asset" << endl;

        const auto fullAssetName = ConstructFullAssetNameInternal<ASSET_TYPE>( assetName );

        if ( m_Assets.find( fullAssetName ) != m_Assets.end() )
        {
            return static_pointer_cast<Asset<ASSET_TYPE>>( m_Assets[ fullAssetName ] );
        }

        AssetMetadata am = {};
        AssetsReader::LoadAsset( fullAssetName + ".b33asset", am, nullptr );

        auto resultVal            = make_shared<Asset<ASSET_TYPE>>( am );
        m_Assets[ fullAssetName ] = resultVal;
        return resultVal;
    }

    unordered_map<string, shared_ptr<AssetWrapper>> GetAssets()
    {
        return m_Assets;
    }

  private:
    template <class ASSET_TYPE>
    string ConstructFullAssetNameInternal( string_view assetName )
    {
        return string( assetName ) + '_' + string( ASSET_TYPE::GetAssetTypeName() );
    }

  private:
    unordered_map<string, shared_ptr<AssetWrapper>> m_Assets = {};
};

// --------------------------------------------------------------------------------------------------------------------
using namespace std;

int main()
{
    cout << "START___" << endl;
    try
    {
        AssetsManager assetsManager = {};

        auto anAsset      = assetsManager.GenerateAsset<Mesh>( "Test" ).lock();
        auto anotherAsset = assetsManager.GenerateAsset<Mesh>( "Test" ).lock();

        anAsset->GetData().Action();
        anotherAsset->GetData().Action();

        // Debug metadata
        {
            auto tt       = AssetMetadata::Clock::to_time_t( anAsset->GetMetadata().CreationTime );
            tm   utc_tm   = *gmtime( &tt );
            tm   local_tm = *localtime( &tt );

            cout << "Metadata" << endl;
            cout << "   Valid: " << anAsset->GetMetadata().Valid << endl;
            cout << "   CreationTime: " << 1900 + local_tm.tm_year << "-" << 1 + local_tm.tm_mon << "-"
                 << local_tm.tm_mday << " " << local_tm.tm_hour << ":" << local_tm.tm_min << ":" << local_tm.tm_sec
                 << endl;
        }

        // Debug assets manager
        {
            cout << "List of loaded assets:" << endl;
            for ( auto &vk : assetsManager.GetAssets() )
                cout << "   " << vk.first << endl;
        }

        assert( anAsset.get() == anotherAsset.get() );
    }
    catch ( ... )
    {
    }
    cout << "___END" << endl;
}
