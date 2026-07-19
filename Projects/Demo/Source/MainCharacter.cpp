#include "MainCharacter.hpp"
#include "MyGame.hpp"

void PaperCharacter::Initialize()
{
    this->SetRotation( ::B33::Math::Vec3 { -0.5f, 1.25f, 0.f } );
    this->SetPositon( ::B33::Math::Vec3 { 14.5f, 2.25f, 25.f } );
}

void PaperCharacter::PlaceBlock( const float )
{
    B33::Math::Vec3 rot     = this->GetRotation();
    B33::Math::Vec3 lookDir = B33::Math::Normalize(
        B33::Math::RotateY( B33::Math::RotateX( B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

    m_g.Lock();

    B33::Rendering::HitResult hr =
        ::B33::Rendering::MarchTheRay( m_g.GetGameInstance().GetWorld().get(), this->GetPosition(), lookDir, 10 );

    if ( hr.bHit )
    {
        auto id = m_g.GetGameInstance().GetIdFromPos( hr.iHitCoords );
        if ( id + 1 )
        {
            const auto halfSize = B33::Math::Vec3( 2.0f, 2.0f, 2.0f ) *
                                  m_g.GetGameInstance().GetWorld()->GetStoredObjects().GetHalfSize(
                                      m_g.GetGameInstance().GetIdFromPos( hr.iHitCoords ) ) *
                                  hr.Normal;
            B33_TRACE( L"GenerateCube, placing on top of cube with halfsizes %f %f %f",
                       halfSize.x,
                       halfSize.y,
                       halfSize.z );
            m_g.GetGameInstance().GenerateCube(
                B33::Math::iVec3( hr.iHitCoords + halfSize ),
                ::B33::Math::Vec3( m_fPlacedType + 0.1f, m_fPlacedType + 0.1f, m_fPlacedType + 0.1f ) );
        }
        else
        {
            m_g.GetGameInstance().GenerateCube(
                B33::Math::iVec3( hr.iHitCoords + hr.Normal ),
                ::B33::Math::Vec3( m_fPlacedType + 0.1f, m_fPlacedType + 0.1f, m_fPlacedType + 0.1f ) );
        }
    }

    m_g.Unlock();
}

void PaperCharacter::RemoveBlock( const float )
{
    B33::Math::Vec3 rot     = this->GetRotation();
    B33::Math::Vec3 lookDir = ::B33::Math::Normalize(
        ::B33::Math::RotateY( ::B33::Math::RotateX( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

    m_g.Lock();

    B33::Rendering::HitResult hr =
        ::B33::Rendering::MarchTheRay( m_g.GetGameInstance().GetWorld().get(), this->GetPosition(), lookDir, 10 );

    if ( hr.bHit )
        m_g.GetGameInstance().RemoveCube( m_g.GetGameInstance().GetIdFromPos( hr.iHitCoords ) );

    m_g.Unlock();
}

void PaperCharacter::Push( const float, const float fForceMul )
{
    B33::Math::Vec3 rot     = this->GetRotation();
    B33::Math::Vec3 lookDir = ::B33::Math::Normalize(
        ::B33::Math::RotateY( ::B33::Math::RotateX( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.x ), rot.y ) );

    m_g.Lock();

    ::B33::Rendering::HitResult hr =
        ::B33::Rendering::MarchTheRay( m_g.GetGameInstance().GetWorld().get(), this->GetPosition(), lookDir, 10 );
    ::B33::Math::Vec3 pushDir = ::B33::Math::Normalize( this->GetPosition() - ::B33::Math::Vec3( hr.iHitCoords ) );
    if ( hr.bHit )
        m_g.GetGameInstance().PushCube( m_g.GetGameInstance().GetIdFromPos( hr.iHitCoords ), pushDir, fForceMul );

    m_g.Unlock();
}

void PaperCharacter::MoveForwardBackwards( const float fDelta, const float fDir )
{
    B33::Math::Rot3 rot     = this->GetRotation();
    B33::Math::Vec3 lookDir = ::B33::Math::RotateY( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.y );

    this->AddPositon( lookDir * fDir * ( fDelta * m_fSpeed ) );
}

void PaperCharacter::Strafe( const float fDelta, float fDir )
{
    B33::Math::Rot3 rot = this->GetRotation();
    B33::Math::Vec3 lookDir =
        ::B33::Math::RotateY( ::B33::Math::Vec3 { 0.f, 0.f, 1.f }, rot.y + ( 90.f * ::B33::Math::B33_DEG_TO_RAD ) );

    this->AddPositon( lookDir * fDir * ( fDelta * m_fSpeed ) );
}

void PaperCharacter::MouseMove( const float, int32_t fX, int32_t fY )
{
    this->AddRotation( B33::Math::Rot3 { 0.00085f * fY, 0.00085f * fX, 0.f } );
}

void PaperCharacter::MoveVertical( const float fDelta, const float dir )
{
    this->AddPositon( ::B33::Math::Vec3 { 0.f, dir * ( fDelta * m_fSpeed ), 0.f } );
}

void PaperCharacter::Move( const float fDelta, const B33::Math::Vec3 &dir )
{
    this->AddPositon( dir * ( fDelta * m_fSpeed ) );
}

void PaperCharacter::ActivateSprint( const float )
{
    m_fSpeed = m_fSprint;
}

void PaperCharacter::ActivateWalk( const float )
{
    m_fSpeed = m_fWalk;
}

void PaperCharacter::RotatePlaceTypeBackward( const float )
{
    m_fPlacedType = std::max( m_fPlacedType - 0.25f, 0.f );
}

void PaperCharacter::RotatePlaceTypeForward( const float )
{
    m_fPlacedType = std::min( m_fPlacedType + 0.25f, 10.f );
}
