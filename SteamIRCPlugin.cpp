#include "SteamIRCPlugin.h"
#include "interface.h"
#include "eiface.h"
#include "convar.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include <stdexcept>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace SteamIRC;
static SteamIRC::IRC* irc = NULL;
static int improvisedSemaphore = 0;

static ConVar irc_nick("irc_nickname", "angryzor", 0, "The nickname you will be using on IRC servers.");
static ConVar irc_user("irc_username", "tf2player1215", 0, "Your username for the IRC server. Is only displayed in WHOIS queries.");
static ConVar irc_real_name("irc_real_name", "Heavy Weapons Guy", 0, "Your real name. Is only displayed in WHOIS queries.");
static ConVar irc_hostname("irc_hostname", "irc.quakenet.org", 0, "The IRC host to connect to on next \"irc_connect\" call.");
static ConVar irc_hostport("irc_host_port", "6667", 0, "Port number to connect to. Mostly in the range of 6666-6669");

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
	Msg(" Build 0009\r\n");
	Msg(" Written by angryzor\r\n");
	Msg("--------------------------------------------------------------\r\n");
	Msg(" Booting...\r\n");
	improvisedSemaphore++;
	try { 
		if(irc)
			throw std::runtime_error("Already an instance of the irc provider running!!!");
		else
			irc = new IRC(interfaceFactory);
	}
	catch(std::runtime_error err) {
		Warning(err.what());
		Warning("Aborting.\r\n");
		DisconnectTier3Libraries( );
		DisconnectTier2Libraries( );
		DisconnectTier1Libraries( );
		return false;
	}

	Msg(" Registering ConVars...\r\n");
	ConVar_Register( 0 );
	Msg(" Boot successful! SteamIRC ready.\r\n");
	Msg("==============================================================\r\n");
	return true;
}

//---------------------------------------------------------------------------------
// Purpose: called when the plugin is unloaded (turned off)
//---------------------------------------------------------------------------------
void CSteamIRCPlugin::Unload( void )
{
	if(irc && (improvisedSemaphore <= 1)) {
		delete irc;
		irc = NULL;
	}

	if(improvisedSemaphore == 1) {
		ConVar_Unregister( );
		DisconnectTier3Libraries( );
		DisconnectTier2Libraries( );
		DisconnectTier1Libraries( );
	}

	improvisedSemaphore--;
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
}


CON_COMMAND( irc_connect, "Connects to an IRC server" )
{
	SteamIRC::IRCUserInfo uInfo = {	irc_nick.GetString(),
									irc_user.GetString(),
									8,
									irc_real_name.GetString()};

	irc->Connect(irc_hostname.GetString(), irc_hostport.GetString(), uInfo);
}

CON_COMMAND( irc_disconnect, "Disconnects from IRC server" )
{
	irc->Disconnect();
}

CON_COMMAND( irc_guishow, "Shows the irc GUI" )
{
	irc->ShowGUI();
}

CON_COMMAND( irc_guihide, "Destroys the irc GUI" )
{
	irc->HideGUI();
}

