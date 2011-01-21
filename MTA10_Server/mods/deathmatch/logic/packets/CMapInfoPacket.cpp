/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/deathmatch/logic/packets/CMapInfoPacket.h
*  PURPOSE:     Map/game information packet class
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Jax <>
*               lil_Toady <>
*               Alberto Alonso <rydencillo@gmail.com>
*               Sebas Lamers <sebasdevelopment@gmx.com>
*
*  Multi Theft Auto is available from http://www.multitheftauto.com/
*
*****************************************************************************/

#include "StdInc.h"

CMapInfoPacket::CMapInfoPacket ( unsigned char ucWeather,
                                 unsigned char ucWeatherBlendingTo,
                                 unsigned char ucBlendedWeatherHour,
                                 unsigned char ucClockHour,
                                 unsigned char ucClockMin,
                                 unsigned long ulMinuteDuration,
                                 bool bShowNametags,
                                 bool bShowRadar,
                                 float fGravity,
                                 float fGameSpeed,
                                 float fWaveHeight,
                                 float fWaterLevel,
                                 bool bHasSkyGradient,
                                 bool* pbGarageStates,
                                 unsigned char ucSkyGradientTR,
                                 unsigned char ucSkyGradientTG,
                                 unsigned char ucSkyGradientTB,
                                 unsigned char ucSkyGradientBR,
                                 unsigned char ucSkyGradientBG,
                                 unsigned char ucSkyGradientBB,
                                 bool bHasHeatHaze,
                                 const SHeatHazeSettings& heatHazeSettings,
                                 unsigned short usFPSLimit,
                                 bool bCloudsEnabled,
                                 float fJetpackMaxHeight,
                                 bool bOverrideWaterColor,
                                 unsigned char ucWaterRed,
                                 unsigned char ucWaterGreen,
                                 unsigned char ucWaterBlue,
                                 unsigned char ucWaterAlpha)
{
    m_ucWeather = ucWeather;
    m_ucWeatherBlendingTo = ucWeatherBlendingTo;
    m_ucBlendedWeatherHour = ucBlendedWeatherHour;
    m_ucClockHour = ucClockHour;
    m_ucClockMin = ucClockMin;
    m_ulMinuteDuration = ulMinuteDuration;
    m_bShowNametags = bShowNametags;
    m_bShowRadar = bShowRadar;
    m_fGravity = fGravity;
    m_fGameSpeed = fGameSpeed;
    m_fWaveHeight = fWaveHeight;
    m_fWaterLevel = fWaterLevel;
    m_bHasSkyGradient = bHasSkyGradient;
    m_pbGarageStates = pbGarageStates;
    m_ucSkyGradientTR = ucSkyGradientTR;
    m_ucSkyGradientTG = ucSkyGradientTG;
    m_ucSkyGradientTB = ucSkyGradientTB;
    m_ucSkyGradientBR = ucSkyGradientBR;
    m_ucSkyGradientBG = ucSkyGradientBG;
    m_ucSkyGradientBB = ucSkyGradientBB;
    m_bHasHeatHaze = bHasHeatHaze;
    m_HeatHazeSettings = heatHazeSettings;
    m_usFPSLimit = usFPSLimit;
    m_bCloudsEnabled = bCloudsEnabled;
    m_fJetpackMaxHeight = fJetpackMaxHeight;
    m_bOverrideWaterColor = bOverrideWaterColor;
    m_ucWaterRed = ucWaterRed;
    m_ucWaterGreen = ucWaterGreen;
    m_ucWaterBlue = ucWaterBlue;
    m_ucWaterAlpha = ucWaterAlpha;
}


bool CMapInfoPacket::Write ( NetBitStreamInterface& BitStream ) const
{
    // Write the map weather
    BitStream.Write ( m_ucWeather );
    BitStream.Write ( m_ucWeatherBlendingTo );
    BitStream.Write ( m_ucBlendedWeatherHour );

    BitStream.WriteBit ( m_bHasSkyGradient );
    if ( m_bHasSkyGradient )
    {
        BitStream.Write ( m_ucSkyGradientTR );
        BitStream.Write ( m_ucSkyGradientTG );
        BitStream.Write ( m_ucSkyGradientTB );
        BitStream.Write ( m_ucSkyGradientBR );
        BitStream.Write ( m_ucSkyGradientBG );
        BitStream.Write ( m_ucSkyGradientBB );
    }

    // Write heat haze
    BitStream.WriteBit ( m_bHasHeatHaze );
    if ( m_bHasHeatHaze )
    {
        SHeatHazeSync heatHaze ( m_HeatHazeSettings );
        BitStream.Write ( &heatHaze );
    }

    // Write the map hour
    BitStream.Write ( m_ucClockHour );
    BitStream.Write ( m_ucClockMin );

    BitStream.WriteCompressed ( m_ulMinuteDuration );

    // Write the map flags
    SMapInfoFlagsSync flags;
    flags.data.bShowNametags  = m_bShowNametags;
    flags.data.bShowRadar     = m_bShowRadar;
    flags.data.bCloudsEnabled = m_bCloudsEnabled;
    BitStream.Write ( &flags );

    // Write any other world conditions
    BitStream.Write ( m_fGravity );
    if ( m_fGameSpeed == 1.0f )
        BitStream.WriteBit ( true );
    else
    {
        BitStream.WriteBit ( false );
        BitStream.Write ( m_fGameSpeed );
    }
    BitStream.Write ( m_fWaveHeight );
    BitStream.Write ( m_fWaterLevel );

    BitStream.WriteCompressed ( m_usFPSLimit );

    // Write the garage states
    for ( unsigned char i = 0 ; i < MAX_GARAGES ; i++ )
    {
        BitStream.WriteBit( static_cast < unsigned char > ( m_pbGarageStates[i] ) );
    }

    // Write the fun bugs state
    SFunBugsStateSync funBugs;
    funBugs.data.bQuickReload = g_pGame->IsGlitchEnabled ( CGame::GLITCH_QUICKRELOAD );
    funBugs.data.bFastFire    = g_pGame->IsGlitchEnabled ( CGame::GLITCH_FASTFIRE );
    funBugs.data.bFastMove    = g_pGame->IsGlitchEnabled ( CGame::GLITCH_FASTMOVE );
    funBugs.data.bCrouchBug   = g_pGame->IsGlitchEnabled ( CGame::GLITCH_CROUCHBUG );
    BitStream.Write ( &funBugs );

    BitStream.Write ( m_fJetpackMaxHeight );

    BitStream.WriteBit ( m_bOverrideWaterColor );
    if ( m_bOverrideWaterColor )
    {
        BitStream.Write ( m_ucWaterRed );
        BitStream.Write ( m_ucWaterGreen );
        BitStream.Write ( m_ucWaterBlue );
        BitStream.Write ( m_ucWaterAlpha );
    }

    return true;
}
