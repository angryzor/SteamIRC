#include "SteamIRCPlugin.h"
#include "interface.h"
#include "filesystem.h"
#include "game/server/iplayerinfo.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "Color.h"
#include "vstdlib/random.h"
#include "engine/IEngineTrace.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "IRCWinSock2.h"
#include "IRCClient.h"
#include "Object.h"
#include "IRCEnvironment.h"
#include "IRCGui.h"
#include <stdexcept>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace SteamIRC;

IRCGui* gui = NULL;
CIRCWinSock2 sockEngine;
CIRCClient* ircClient = NULL;
CIRCEnvironment ircEnv;
HANDLE tRecv = NULL;
bool runRecv = false;

CSteamIRCPlugin::CSteamIRCPlugin(void) : m_iClientCommandIndex(0)
{
}

CSteamIRCPlugin::~CSteamIRCPlugin(void)
{
}

bool CSteamIRCPlugin::Load(	CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory )
{
	// Connect tiers
	ConnectTier1Libraries( &interfaceFactory, 1 );
	ConnectTier2Libraries( &interfaceFactory, 1 );
	ConnectTier3Libraries( &interfaceFactory, 1 );

	Msg("======================= SteamIRC v0.1a =======================\r\n");
	Msg(" Build 0008\r\n");
	Msg(" Written by angryzor\r\n");
	Msg("--------------------------------------------------------------\r\n");
	Msg(" Booting...\r\n");
	Msg(" Starting GUI system...\r\n");

	try { 
		gui = new IRCGui(interfaceFactory, ircEnv); 
	}
	catch(std::runtime_error err) {
		Warning(err.what());
		Warning("Aborting.");
		return false;
	}
	ircEnv.SetGui(gui);

	Msg(" Registering ConVars...\r\n");
	ConVar_Register( 0 );

//	InitializeCriticalSection(&csRecv);

	ircClient = sockEngine.MakeIRCClient(ircEnv);

	Msg(" Boot successful! SteamIRC ready.\r\n");
	return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::Unload( void )
{

	delete ircClient;
	ircClient = NULL;

	gui->DestroyPanel();
	delete gui;
//	DeleteCriticalSection(&csRecv);

	ConVar_Unregister( );
	DisconnectTier3Libraries( );
	DisconnectTier2Libraries( );
	DisconnectTier1Libraries( );
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is paused (i.e should stop running but isn't unloaded)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::Pause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unpaused (i.e should start executing again)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::UnPause( void )
{
}

//---------------------------------------------------------------------------------
// Purpose: the name of this plugin, returned in "plugin_print" command
//---------------------------------------------------------------------------------
const char *CSteamIRCPlugin::GetPluginDescription( void )
{
	return "SteamIRC - IRC Client for Steam";
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::LevelInit( char const *pMapName )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start, when the server is ready to accept client connections
//		edictCount is the number of entities in the level, clientMax is the max client count
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
}

//---------------------------------------------------------------------------------
// Purpose: called once per server frame, do recurring work here (like checking for timeouts)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::GameFrame( bool simulating )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level end (as the server is shutting down or going to a new map)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::LevelShutdown( void ) // !!!!this can get called multiple times per map change
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client spawns into a server (i.e as they begin to play)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::ClientActive( edict_t *pEntity )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client leaves a server (or is timed out)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::ClientDisconnect( edict_t *pEntity )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on 
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::ClientPutInServer( edict_t *pEntity, char const *playername )
{
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::SetCommandClient( int index )
{
	m_iClientCommandIndex = index;
}

//---------------------------------------------------------------------------------
// Purpose: called on level start
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::ClientSettingsChanged( edict_t *pEdict )
{
}

//---------------------------------------------------------------------------------
// Purpose: called when a client joins a server
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSteamIRCPlugin::ClientConnect( bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a client types in a command (only a subset of commands however, not CON_COMMAND's)
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSteamIRCPlugin::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	return PLUGIN_CONTINUE; 
}

//---------------------------------------------------------------------------------
// Purpose: called when a client is authenticated
//---------------------------------------------------------------------------------
PLUGIN_RESULT CSteamIRCPlugin::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	return PLUGIN_CONTINUE;
}

//---------------------------------------------------------------------------------
// Purpose: called when a cvar value query is finished
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	Msg( "Cvar query (cookie: %d, status: %d) - name: %s, value: %s\n", iCookie, eStatus, pCvarName, pCvarValue );
}

DWORD WINAPI recvthread(void* lpThreadParameter)
{
	while(runRecv)
	{
		if(ircClient->CheckRecv())
			ircClient->DoRecv();
		else
			Sleep(500);
	}
	return EXIT_SUCCESS;
}

CON_COMMAND( irc_connect, "Connects to an IRC server" )
{
	try
	{
		tRecv = NULL;
		IRCUserInfo uInfo = { args[3],
							  args[4],
							  8,
							  args[5] };

		// Connect our client
		ircClient->Connect(args[1], args[2], uInfo);

		// Create the receive thread
		runRecv = true;
		tRecv = CreateThread(NULL, 0, &recvthread, 0, 0, NULL);
	}
	catch(std::runtime_error err)
	{
		Warning(err.what());
	}
}

CON_COMMAND( irc_chat4allconnect, "Debug - Connects to Chat4all IRC server" )
{
	try
	{
		tRecv = NULL;
		IRCUserInfo uInfo = { "Ruben",
							  "ruben_tytgat",
							  3,
							  "Ruben Tytgat" };

		// Connect our client
		ircClient->Connect("irc.chat4all.org", "6667", uInfo);

		// Create the receive thread
		runRecv = true;
		tRecv = CreateThread(NULL, 0, &recvthread, 0, 0, NULL);
	}
	catch(std::logic_error err)
	{
		Warning(err.what());
	}
}

CON_COMMAND( irc_disconnect, "Disconnects from IRC server" )
{
	// Alert the recvthread that it should stop processing messages and wait for it to finish
	runRecv = false;
	WaitForSingleObject(tRecv,INFINITE);

	ircClient->Disconnect();
}

CON_COMMAND( irc_join, "Joins a channel" )
{
}










CON_COMMAND( irc_show, "shows the irc window" )
{

	try {
		gui->CreatePanel();
	}
	catch(std::logic_error err) {
		Warning(err.what());
		return;
	}
	Msg( "Completed.\n" );
}

CON_COMMAND( irc_unshow, "destroys the irc window" )
{
	Msg( "deleting" );

	gui->DestroyPanel();
	Msg( "Completed." );
}
