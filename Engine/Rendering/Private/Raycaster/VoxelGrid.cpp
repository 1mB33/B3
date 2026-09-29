#include "B33Rendering.h"

#include "Raycaster/VoxelGrid.hpp"

namespace B33::Rendering
{

using namespace ::std;
using namespace ::B33::Math;

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::SetVoxel( const iVec &pos, u32 uColor )
{
    vector<Voxel> &voxelsGrid = this->GetGrid();
    const usize    uIndex     = CalcIndex( pos );

    B33_ASSERT( uIndex < voxelsGrid.size() );

    voxelsGrid[ uIndex ].Type  = Voxel::FullSolid;
    voxelsGrid[ uIndex ].Color = uColor;
    this->ForceUpload();
}

// --------------------------------------------------------------------------------------------------------------------
usize IWorldGrid::CalcIndex( const iVec &pos ) const
{
    return pos.x + pos.y * m_uGridDim + pos.z * m_uGridDim * m_uGridDim;
}

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::PlaceOnGrid( const iVec &pos, const iVec &area, const usize uId )
{
    // Incremeant the type on connected voxels'
    usize uCornerIndex;
    for ( i32 x = -area.x; x <= area.x; ++x )
    {
        for ( i32 y = -area.y; y <= area.y; ++y )
        {
            for ( i32 z = -area.z; z <= area.z; ++z )
            {
                uCornerIndex = CalcIndex( iVec( pos.x + x, pos.y + y, pos.z + z ) );

                if ( uCornerIndex >= m_VoxelGrid.size() )
                    continue;

                Voxel &voxel = m_VoxelGrid[ uCornerIndex ];

                if ( voxel.Type == Voxel::FullSolid )
                    continue;

                if ( voxel.Type >= Voxel::MaxPerInstance )
                {
                    B33_LOG( Core::Debug::Warning, L"Reached object limit for the connected voxel" );
                    continue;
                }

                voxel.Id[ voxel.Type++ ] = uId;
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void IWorldGrid::RemoveFromGrid( const iVec &pos, const iVec &area, const usize uId )
{
    // Decremeant the type on connected voxels
    usize                                 uCornerIndex;
    __B33_ATTRIBUTE_MIGHT_BE_UNUSED usize uLastIdOnList;
    usize                                 uIndexOfIdOnList;
    for ( i32 x = -area.x; x <= area.x; ++x )
    {
        for ( i32 y = -area.y; y <= area.y; ++y )
        {
            for ( i32 z = -area.z; z <= area.z; ++z )
            {
                uCornerIndex = CalcIndex( iVec( pos.x + x, pos.y + y, pos.z + z ) );

                if ( uCornerIndex >= m_VoxelGrid.size() )
                    continue;

                Voxel &voxel = m_VoxelGrid[ uCornerIndex ];

                if ( voxel.Type == Voxel::FullSolid || voxel.Type == 0 )
                    continue;

                uLastIdOnList = voxel.Id[ voxel.Type - 1 ];

                for ( uIndexOfIdOnList = 0; uIndexOfIdOnList < voxel.Type; ++uIndexOfIdOnList )
                    if ( voxel.Id[ uIndexOfIdOnList ] == uId )
                        break;

                voxel.Id[ uIndexOfIdOnList ] = voxel.Id[ --voxel.Type ];
            }
        }
    }
}

} // namespace B33::Rendering
