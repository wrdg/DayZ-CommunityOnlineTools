#ifndef CF_MODULE_PERMISSIONS
class JMPlayerSerialize : Managed
{
	[NonSerialized()]
	string m_FileName;

	ref array< string > Roles;

	void JMPlayerSerialize()
	{
		Roles = new array< string >;
	}

	void ~JMPlayerSerialize()
	{
		delete Roles;
	}

	static string FileReadyStripName( string name )
	{
		name.Replace( "\\", "" );
		name.Replace( "/", "" );
		name.Replace( "=", "" );
		name.Replace( "+", "" );

		return name;
	}

	static bool Load( JMPlayerInstance inst, inout JMPlayerSerialize playerFile )
	{
		playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER;
		if ( FileExist( playerFile.m_FileName ) )
		{
			JsonFileLoader<JMPlayerSerialize>.JsonLoadFile( playerFile.m_FileName, playerFile );

			playerFile.Save();
			return true;
		}
		
		playerFile.m_FileName = JMConstants.DIR_PLAYERS + inst.GetSteam64ID() + JMConstants.EXT_PLAYER;
		if ( FileExist( playerFile.m_FileName ) )
		{
			JsonFileLoader<JMPlayerSerialize>.JsonLoadFile( playerFile.m_FileName, playerFile );

			DeleteFile( playerFile.m_FileName );
			playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER;

			playerFile.Save();
			return true;
		}
		
		playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER + JMConstants.EXT_WINDOWS_DEFAULT;
		if ( FileExist( playerFile.m_FileName ) )
		{
			JsonFileLoader<JMPlayerSerialize>.JsonLoadFile( playerFile.m_FileName, playerFile );

			DeleteFile( playerFile.m_FileName );
			playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER;

			playerFile.Save();
			return true;
		}

		playerFile.m_FileName = JMConstants.DIR_PLAYERS + inst.GetSteam64ID() + JMConstants.EXT_PLAYER + JMConstants.EXT_WINDOWS_DEFAULT;
		if ( FileExist( playerFile.m_FileName ) )
		{
			JsonFileLoader<JMPlayerSerialize>.JsonLoadFile( playerFile.m_FileName, playerFile );

			DeleteFile( playerFile.m_FileName );
			playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER;

			playerFile.Save();
			return true;
		}

		playerFile.m_FileName = JMConstants.DIR_PLAYERS + FileReadyStripName( inst.GetGUID() ) + JMConstants.EXT_PLAYER;
		playerFile.Roles.Insert( "everyone" );
		return false;
	}

	void Save()
	{
		JsonFileLoader<JMPlayerSerialize>.JsonSaveFile( m_FileName, this );
	}
};
#endif