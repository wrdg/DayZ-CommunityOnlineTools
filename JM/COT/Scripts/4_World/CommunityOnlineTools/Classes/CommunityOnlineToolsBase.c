class CommunityOnlineToolsBase
{
	private bool m_Loaded;

	private bool m_IsActive;
	private bool m_IsOpen;

	private string m_FileLogName;

	protected JMWebhookModule m_Webhook;

	void CommunityOnlineToolsBase()
	{
		m_Loaded = false;

		GetDayZGame().Event_OnRPC.Insert( OnRPC );
	}

	void ~CommunityOnlineToolsBase()
	{
		array< JMRenderableModuleBase > cotModules = GetModuleManager().GetCOTModules();
		for ( int i = 0; i < cotModules.Count(); i++ )
		{
			cotModules[i].Hide();
		}

		GetDayZGame().Event_OnRPC.Remove( OnRPC );
	}

	void CreateNewLog()
	{
		if ( !FileExist( JMConstants.DIR_LOGS ) )
		{
			MakeDirectory( JMConstants.DIR_LOGS );
		}

		m_FileLogName = JMConstants.DIR_LOGS + "cot-" + JMDate.Now( true ).ToString( "YYYY-MM-DD-hh-mm-ss" ) + JMConstants.EXT_LOG;
		int fileLog = OpenFile( m_FileLogName, FileMode.WRITE );

		if ( fileLog != 0 )
		{
			CloseFile( fileLog );
		}
	}

	void CloseLog()
	{
		m_FileLogName = "";
	}

	void OnStart()
	{
		m_IsOpen = false;

		JMScriptInvokers.COT_ON_OPEN.Invoke( m_IsOpen );
		
		#ifndef CF_MODULE_PERMISSIONS
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
		{
			GetPermissionsManager().LoadRoles();
		}
		#endif

		CreateNewLog();

		if ( IsMissionOffline() )
		{
			m_Webhook = NULL;
		} else
		{
			Class.CastTo( m_Webhook, GetModuleManager().GetModule( JMWebhookModule ) );
		}
	}

	void OnFinish()
	{
		m_IsOpen = false;

		JMScriptInvokers.COT_ON_OPEN.Invoke( m_IsOpen );

		CloseLog();
	}

	void OnLoaded()
	{
	}

	void OnUpdate( float timeslice )
	{
		if ( !m_Loaded && !GetDayZGame().IsLoading() )
		{
			m_Loaded = true;
			OnLoaded();
		}
	}

	bool IsActive()
	{
		return m_IsActive;
	}

	void SetActive( bool active )
	{
		m_IsActive = active;

		OnCOTActiveChanged( m_IsActive );
	}

	void ToggleActive()
	{
		m_IsActive = !m_IsActive;

		OnCOTActiveChanged( m_IsActive );
	}

	void OnCOTActiveChanged( bool active )
	{
	}

	bool IsOpen()
	{
		return m_IsOpen;
	}

	void SetOpen( bool open )
	{
		if ( open )
		{
			if ( GetGame().GetUIManager().GetMenu() )
			{
				return;
			}

			if ( !GetPermissionsManager().HasPermission( "COT.View" ) )
			{
				return;
			}

			if ( !GetCommunityOnlineToolsBase().IsActive() )
			{
				COTCreateLocalAdminNotification( new StringLocaliser( "STR_COT_NOTIFICATION_WARNING_TOGGLED_OFF" ) );
				return;
			}
		}

		m_IsOpen = open;

		JMScriptInvokers.COT_ON_OPEN.Invoke( m_IsOpen );
	}

	void ToggleOpen()
	{
		if ( !m_IsOpen )
		{
			if ( GetGame().GetUIManager().GetMenu() )
			{
				return;
			}

			if ( !GetPermissionsManager().HasPermission( "COT.View" ) )
			{
				return;
			}

			if ( !GetCommunityOnlineToolsBase().IsActive() )
			{
				COTCreateLocalAdminNotification( new StringLocaliser( "STR_COT_NOTIFICATION_WARNING_TOGGLED_OFF" ) );
				return;
			}
		}

		m_IsOpen = !m_IsOpen;

		JMScriptInvokers.COT_ON_OPEN.Invoke( m_IsOpen );
	}

	void LogServer( string text )
	{
		if ( GetGame().IsServer() )
		{
			GetGame().AdminLog( "[COT] " + text );
		}

		int fileLog = OpenFile( m_FileLogName, FileMode.APPEND );
		if ( fileLog != 0 )
		{
			FPrintln( fileLog, "[COT " + JMDate.Now( true ).ToString( "YYYY-MM-DD hh:mm:ss" ) + "] " + text );
			CloseFile( fileLog );
		}
	}

	void Log( JMPlayerInstance logInstacPlyer, string text )
	{
		if ( GetGame().IsMultiplayer() )
		{
			text = "" + logInstacPlyer.GetSteam64ID() + ": " + text;
		} else
		{
			text = "Offline: " + text;
		}

		if ( GetGame().IsServer() )
		{
			GetGame().AdminLog( "[COT] " + text );
		}

		int fileLog = OpenFile( m_FileLogName, FileMode.APPEND );
		if ( fileLog != 0 )
		{
			FPrintln( fileLog, "[COT " + JMDate.Now( true ).ToString( "YYYY-MM-DD hh:mm:ss" ) + "] " + text );
			CloseFile( fileLog );
		}
	}

	void Log( PlayerIdentity logIdentPlyer, string text )
	{
		if ( GetGame().IsMultiplayer() )
		{
			text = "" + logIdentPlyer.GetPlainId() + ": " + text;
		} else
		{
			text = "Offline: " + text;
		}

		if ( GetGame().IsServer() )
		{
			GetGame().AdminLog( "[COT] " + text );
		}

		int fileLog = OpenFile( m_FileLogName, FileMode.APPEND );
		if ( fileLog != 0 )
		{
			FPrintln( fileLog, "[COT " + JMDate.Now( true ).ToString( "YYYY-MM-DD hh:mm:ss" ) + "] " + text );
			CloseFile( fileLog );
		}
	}

	void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
	}

	void UpdateRole( JMRole role, PlayerIdentity toSendTo )
	{
	}

	void UpdateClient( string guid, PlayerIdentity sendTo )
	{
	}

	void RemoveClient( string guid )
	{
	}

	void RefreshClients()
	{
	}

	void RefreshClientPositions()
	{
	}

	void SetClient( JMPlayerInstance player )
	{
	}

	void SetClient( JMPlayerInstance player, PlayerIdentity identity )
	{
	}

	static void ForceDisableInputs(bool state, inout TIntArray skipIDs = null)
	{
		if (!skipIDs)
			skipIDs = new TIntArray;

		skipIDs.Insert(UAUIBack);

		TIntArray inputIDs = new TIntArray;

		GetUApi().GetActiveInputs(inputIDs);

		foreach (int inputID: inputIDs)
		{
			if (skipIDs.Find(inputID) == -1)
			{
				GetUApi().GetInputByID(inputID).ForceDisable(state);
			}
		}
	}

	static void HealEntityRecursive(EntityAI entity, bool includeAttachments = true, bool includeCargo = true)
	{
		entity.SetFullHealth();

		int i;

		if (includeAttachments)
		{
			for (i = 0; i < entity.GetInventory().AttachmentCount(); i++)
			{
				HealEntityRecursive(entity.GetInventory().GetAttachmentFromIndex(i), true, includeCargo);
			}
		}

		if (includeCargo)
		{
			CargoBase cargo = entity.GetInventory().GetCargo();
			if (cargo)
			{
				for (i = 0; i < cargo.GetItemCount(); i++)
				{
					HealEntityRecursive(cargo.GetItem(i), includeAttachments, true);
				}
			}
		}
	}
}

static ref CommunityOnlineToolsBase g_cotBase;

static CommunityOnlineToolsBase GetCommunityOnlineToolsBase()
{
	return g_cotBase;
}