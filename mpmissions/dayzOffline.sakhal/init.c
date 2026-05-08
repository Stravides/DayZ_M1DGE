/*
* Custom Map addons.
*/
// #include "$CurrentDir:\\mpmissions\\dayzOffline.chernarusplus\\map_addons\\CustomSpawns.c"


//Spawn helper function
void SpawnObject(string objType, vector objPos, vector objOrientation)
{
	Object m_Building = Object.Cast(GetGame().CreateObject(objType, objPos));
	if (m_Building == null) return;
	m_Building.SetAffectPathgraph(true, false);
	GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, m_Building);
	m_Building.SetPosition(objPos);
	m_Building.SetOrientation(objOrientation);
}

void main()
{
//	CustomSpawns();			//Trader Billboards and ATMs



	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 2, reset_day = 1;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}


	// *** LOOT *** - uncomment last line after step 4.line to create loot for newly placed buildings
	// 1. Run server with the line below uncommented
	// 2. It will generate a new mapgrouppos.xml (loot-spots) in /dayzOffline.chernarusplus/storage_1/export
	// 3. Copy this to /dayzOffline.chernarusplus/mapgrouppos.xml replacing the old.
	// 4. Comment out line below and restart server
	//	GetCEApi().ExportProxyData( "7500 0 7500", 10000 );
 	// GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);



class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.65, 0.95 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}
	
	void SetLowHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomFloat( 0.35, 0.55 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}
	
	void SetQuantity(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			float rndHlt = Math.RandomInt( 1, 5 );
			itemEnt.SetQuantity(rndHlt);
		}
	}

	void SetHealth(EntityAI itemEnt, int health)
	{
		itemEnt.SetHealth("","",health);
	}
	
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer( identity, characterName, pos, 0, "NONE" );
		Class.CastTo( m_player, playerEnt );

/* 		g_Game.SendMessage(
		false,
		identity,
		"Server Login:",
		"Welcome "+identity.GetName() + ", enjoy your stay! - Press Pause/Break Button for Server info",
		10,
		2,
		false,
		true,
		"",
		0,
		0);
 */
		GetGame().SelectPlayer( identity, m_player );

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{	
		TStringArray admin_ids = {
			"76561198015455963", // stravides
			"76561198054709847" // hammersmith
			
		};

		TStringArray donator_ids = {
			"76561198015455963", // stravides
			"76561198054709847", // hammersmith
			"76561197972431045"  // User
		};

		bool IsAdmin = false;
		bool IsDonator = false;
		string plainId = player.GetIdentity().GetPlainId();

		if ( admin_ids.Find(plainId) != -1 )
		{
			IsAdmin = true;
			IsDonator = true;
		}
		else if ( donator_ids.Find(plainId) != -1 )
		{
			IsDonator = true;
		}

		//Donator Equipment
    TStringArray D_tops			= {"TTsKOJacket_Camo"};
		TStringArray D_pants 		= {"TTSKOPants"};
    TStringArray D_shoes 		= {"CombatBoots_Black","MilitaryBoots_Redpunk"};
		TStringArray D_vest 		= {"SmershVest"};

		//Normals Spawn Stuff
    TStringArray tops				= {"Shirt_PlaneBlack","UKP_Tshirt_DH_White","UKP_Tshirt_DH_Red","UKP_Tshirt_DH_Grey","UKP_Tshirt_DH_Green","UKP_Tshirt_DH_Blue","UKP_Tshirt_DH_Black","UKP_Tshirt_DH_Beige","UKP_Tshirt_Skull_White","UKP_Tshirt_Skull_Grey","UKP_Tshirt_Skull_Black","UKP_Tshirt_DF_White","UKP_Tshirt_DF_Red","UKP_Tshirt_DF_Green","UKP_Tshirt_DF_Blue","UKP_Tshirt_DF_Black","UKP_Tshirt_DF_Beige","UKP_Tshirt_Strav_White","UKP_Tshirt_Strav_Blue","UKP_Tshirt_Strav_Black","UKP_Tshirt_Strav_Beige","TShirt_White"};
		TStringArray pants 			= {"Jeans_Black","Jeans_BlueDark","Jeans_Blue","Jeans_Brown","Jeans_Green","Jeans_Grey"};
    TStringArray shoes 			= {"AthleticShoes_Black","AthleticShoes_Blue","AthleticShoes_Brown","AthleticShoes_Green","AthleticShoes_Grey","HikingBootsLow_Beige","HikingBootsLow_Black","HikingBootsLow_Blue","HikingBootsLow_Grey","HikingBoots_Black","HikingBoots_Brown"};
    TStringArray backpack		= {"ImprovisedBag","FurImprovisedBag","simplebackpack_camoblack_mung","simplebackpack_redpink_mung","simplebackpack_redyellow_mung"};
    TStringArray medic 			= {"Rags","BandageDressing"};
    TStringArray drink 			= {"SodaCan_Cola","SodaCan_Kvass","SodaCan_Pipsi","SodaCan_Spite"};
    TStringArray food 			= {"Apple","Plum","Pear"};	
			
		EntityAI itemEnt;	
		EntityAI itemIn;
		ItemBase itemBs;

		if (IsDonator)
		{
			// Donators

			player.RemoveAllItems();
		
			EntityAI item5 = player.GetInventory().CreateInInventory(D_tops.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);
			
			EntityAI item6 = player.GetInventory().CreateInInventory(D_pants.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);
				
			EntityAI item7 = player.GetInventory().CreateInInventory(D_shoes.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20)
			
//			EntityAI item8 = player.GetInventory().CreateInInventory(D_vest.GetRandomElement());
//			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);
		
			itemEnt = player.GetInventory().CreateInInventory("HuntingKnife");     				// added items
			itemBs = ItemBase.Cast(itemEnt);
			SetRandomHealth(itemEnt);
			
			if (IsAdmin)
			{
				itemEnt = player.GetInventory().CreateInInventory("CombatKnife");     			// added items
				itemBs = ItemBase.Cast(itemEnt);
				SetRandomHealth(itemEnt);
			}
		} 
		else 
		{
			player.RemoveAllItems();  
			
			EntityAI item = player.GetInventory().CreateInInventory(tops.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);
			
			EntityAI item2 = player.GetInventory().CreateInInventory(pants.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);
				
			EntityAI item3 = player.GetInventory().CreateInInventory(shoes.GetRandomElement());
			itemBs = ItemBase.Cast(itemEnt);
//			SetHealth(itemEnt, 20);	
		
			itemEnt = player.GetInventory().CreateInInventory("StoneKnife");     			// added items
			itemBs = ItemBase.Cast(itemEnt);
//			SetRandomHealth(itemEnt);			
		}

		EntityAI item4 = player.GetInventory().CreateInInventory(backpack.GetRandomElement());
		itemBs = ItemBase.Cast(itemEnt);
//		SetHealth(itemEnt, 20);
		
		itemEnt = player.GetInventory().CreateInInventory(medic.GetRandomElement());
		itemBs = ItemBase.Cast(itemEnt);
//		SetRandomHealth(itemEnt);

		itemEnt = player.GetInventory().CreateInInventory(drink.GetRandomElement());
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);

		itemEnt = player.GetInventory().CreateInInventory(food.GetRandomElement());
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);
		
		itemEnt = player.GetInventory().CreateInInventory("Marmalade");    				// added items
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);
		
		itemEnt = player.GetInventory().CreateInInventory("Compass");        			// added items
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);
		
		itemEnt = player.GetInventory().CreateInInventory("Heatpack");       			// added items
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);
		
		itemEnt = player.GetInventory().CreateInInventory("TM_Wallet");       			// added items
		itemBs = ItemBase.Cast(itemEnt);
		SetRandomHealth(itemEnt);

	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}