using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;
using System.Xml.Linq;
using SGP;


namespace World_Editor
{
    public partial class Form1 : Form
    {
        // World Editor Data Member Declaration ////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////
        enum LayerType      { FRONT_LAYER = 1, PLAY_LAYER, MID_LAYER, BACK_LAYER };
        enum CollideType    { NONCOLLIDE = -1, COLLIDE = 1, GLASS, JUMP, STOP, GRAVITYON };

        enum PowerupType { GREASED_PIG = 1, CHEETAH, POGO, FLUBBER, FIZZY };
        enum ToolType { SQUIRT = 0, BALLOON, COTTON_CANDY };
        enum GateType { CLOSED = 0, OPEN };


        ManagedDirect3D         mD3D    = ManagedDirect3D.Instance;
        ManagedTextureManager   mTM     = ManagedTextureManager.Instance;

        

        public bool Looping = true;

        public bool bPlayLayer = false;
        public bool bBackLayer = false;
        public bool bFrontLayer = false;
        public bool bMidLayer = false;
        public bool bPlayBrush = false;
        public bool bFrontBrush = false;
        public bool bMidBrush = false;
        public bool bJumpNodeBrush = false;
        public bool bStopNodeBrush = false;
        public bool bGravityFlipBrush = false;
        public bool bIsFoxIn = false;
        public bool bIsCorinneIn = false;
        public bool bGateOpen = false;

        public bool bEraseTileBrush = false;
        public bool bEraseElementBrush = false;
        public bool bEndPointBrush = false;
        public Size sEndPointSize = new Size();
        public Rectangle rEndPointRect = new Rectangle( );

        public bool bCollisionBrush = false;
        public bool bBeginLayer = true;
        public bool bIsTileOutsideMap = false;

        public string szCurrentTrigger = string.Empty;

        string currentPath;
        string IMGfilename;
        string foreignPath = Directory.GetCurrentDirectory();

        public Point winPoint = new Point(0, 0);

        Color ColorElementButton = Color.FromArgb(255, 233, 233, 111);

        // Tileset data members
        int         nTileSetID = -1;
        Rectangle   rTileRect = new Rectangle( 0, 0, 1024, 512 );
        // Selected Tile in Tileset
        public Point selectedTile = new Point(0, 0);
        public Point selectedEndTile = new Point(32, 32);
        public bool bDragging = false;
        public Point selectedWorldDrag = new Point(-32, -32);
        
        // Define mapsize, Tileset grid size, tile size for editor.
        public Size mapSize = new Size(25, 19);
        public Size tileSetSize = new Size(23, 44);
        public Size tileSize = new Size(32, 32);

        // Declaring a Tile Map class to store all Tile data
        public class tileMap
        {
            // All Object Positions, X and Y
            public Point[,]         map             = new Point[25, 19];
            // All Object Layertypes
            public int[,]           nLayerType      = new int[25, 19];
            // All Play Layer Rects
            public Rectangle[,]     rPlayRect          = new Rectangle[25, 19];
            // All Front Layer Rects
            public Rectangle[,]     rFrontRect = new Rectangle[25, 19];
            // All MidBack Layer Rects
            public Rectangle[,]     rMidBackRect = new Rectangle[25, 19];
            // All Play Layer ImageIDs
            public int[,]           nPlayTileImageID = new int[25, 19];
            // All Front Layer ImageIDs
            public int[,]           nFrontTileImageID = new int[25, 19];
            // All MidBack Layer ImageIDs
            public int[,]           nMidBackTileImageID = new int[25, 19];

            public string[,]        szEventID         = new string[ 25,19];

            public string[,]        szTriggerEventID = new string[25, 19];
            // Tile Object Collides
            public int[,]           nCollideType    = new int[25, 19];

        };

        // Declaring a Element Event class to store a Game Element data
        public class CElementEvent
        {
            public string szEvent;
            public bool bBrush;
            public int nStringIndiceSize;

            public string szFilename;
            public Size sImageSize;
            public int nImageID;
            public Rectangle rIRect;

            public int nLayerType;
            
            public int nPowerupType;
            public int nToolType;
            public string szTriggerEvent;

            public int nPatrolDistance;



            public CElementEvent() { nLayerType = (int)LayerType.PLAY_LAYER; }

        };

        // Declaraing a Element Library class to store all Element types
        public class CElementLibrary
        {
            // Element Objects
            public CElementEvent BackgroundIMG;

            public CElementEvent LeverElement;
            public CElementEvent HorizontalGateElement;
            public CElementEvent VerticalGateElement;
            public CElementEvent PressurePlateElement;

            public CElementEvent CameraElement;
            public CElementEvent SpringElement;
            public CElementEvent BalloonElement;
            public CElementEvent CrateElement;

            public CElementEvent PogoElement;
            public CElementEvent CheetahElement;
            public CElementEvent FizzyElement;
            public CElementEvent GreasedPigElement;
            public CElementEvent FlubbElement;

            public CElementEvent WaterGunElement;
            public CElementEvent CottonCandyElement;
            public CElementEvent BalloonGunElement;

            public CElementEvent RatElement;
            public CElementEvent ClownElement;
            public CElementEvent HoodlumElement;

            public CElementEvent CheckpointElement;

            public CElementEvent CorinneElement;
            public CElementEvent FoxElement;

        };

        // Initializing Tile Map class.
        public tileMap c_TM = new tileMap();
        // Initializing Element Library.
        public CElementLibrary c_EL = new CElementLibrary();

        // Application /////////////////////////////////////////////////////////////////////////////////
        public Form1()
        {
            InitializeComponent( );
            ScrollingSizeSet( );

            // Init D3D and D3DTM
            mD3D.InitManagedDirect3D( Tileset_Panel );
            mD3D.InitManagedDirect3D( Tile_World_Panel );
            mTM.InitManagedTextureManager( mD3D.Device, mD3D.Sprite );
            
            // Setting Flickering Off
            this.DoubleBuffered = true;

            // Relative Paths? //
            // <-----------------
            currentPath = Directory.GetCurrentDirectory();

            // Default Tile Set image loading from Masquerade Game Resources folder.
            IMGfilename = "MASQ_Tileset.png";
            nTileSetID = mTM.LoadTexture( currentPath + "\\..\\..\\Resources\\" + IMGfilename, 
                Color.FromArgb(255, 255, 255, 255).ToArgb() );

            // Initializing necessary Tile Map data to invalid/default values
            for (int x = 0; x < mapSize.Width; x++)
            {
                for (int y = 0; y < mapSize.Height; y++)
                {
                    c_TM.nPlayTileImageID[ x, y ] = -1;
                    c_TM.nMidBackTileImageID[x, y] = -1;
                    c_TM.nFrontTileImageID[x, y] = -1;

                    c_TM.nCollideType[x, y] = -1;
                    c_TM.nLayerType[x, y] = 0;
                }
            }

            // Setting Tileset and Tile Sizes to the Up-Down Buttons
            numericUpDown1.Value = (int)tileSize.Width;
            numericUpDown2.Value = (int)tileSize.Height;

            numericUpDown3.Value = (int)tileSetSize.Width;
            numericUpDown4.Value = (int)tileSetSize.Height;


            // All Image Elements Initialization
            // Background Object
            c_EL.BackgroundIMG = new CElementEvent();
            c_EL.BackgroundIMG.nImageID = -1;
            c_EL.BackgroundIMG.rIRect = new Rectangle();
            c_EL.BackgroundIMG.sImageSize = new Size();
            c_EL.BackgroundIMG.nLayerType = (int)LayerType.BACK_LAYER;

            // Puzzle Objects
            c_EL.HorizontalGateElement = new CElementEvent();
            c_EL.HorizontalGateElement.szEvent = "Horz_Gate_";
            c_EL.HorizontalGateElement.nImageID = -1;
            c_EL.HorizontalGateElement.rIRect.X = 0;
            c_EL.HorizontalGateElement.rIRect.Y = 0;
            c_EL.HorizontalGateElement.rIRect.Width = 128;
            c_EL.HorizontalGateElement.rIRect.Height = 32;
            c_EL.HorizontalGateElement.sImageSize = new Size( 160, 32 );
            c_EL.HorizontalGateElement.bBrush = false;
            c_EL.HorizontalGateElement.nStringIndiceSize = 1;
            c_EL.HorizontalGateElement.szTriggerEvent = string.Empty;

            c_EL.VerticalGateElement = new CElementEvent();
            c_EL.VerticalGateElement.szEvent = "Vert_Gate_";
            c_EL.VerticalGateElement.nImageID = -1;
            c_EL.VerticalGateElement.rIRect.X = 0;
            c_EL.VerticalGateElement.rIRect.Y = 0;
            c_EL.VerticalGateElement.rIRect.Width = 96;
            c_EL.VerticalGateElement.rIRect.Height = 224;
            c_EL.VerticalGateElement.sImageSize = new Size(96, 224);
            c_EL.VerticalGateElement.bBrush = false;
            c_EL.VerticalGateElement.nStringIndiceSize = 1;
            c_EL.VerticalGateElement.szTriggerEvent = string.Empty;

            c_EL.PressurePlateElement = new CElementEvent();
            c_EL.PressurePlateElement.szEvent = "Pressure_Plate_";
            c_EL.PressurePlateElement.nImageID = -1;
            c_EL.PressurePlateElement.rIRect.X = 0;
            c_EL.PressurePlateElement.rIRect.Y = 0;
            c_EL.PressurePlateElement.rIRect.Width = 64;
            c_EL.PressurePlateElement.rIRect.Height = 64;
            c_EL.PressurePlateElement.sImageSize = new Size(64, 64);
            c_EL.PressurePlateElement.bBrush = false;
            c_EL.PressurePlateElement.nStringIndiceSize = 1;
            
            c_EL.CameraElement = new CElementEvent();
            c_EL.CameraElement.szEvent = "Security_Cam_";
            c_EL.CameraElement.nImageID = -1;
            c_EL.CameraElement.rIRect.X = 0;
            c_EL.CameraElement.rIRect.Y = 0;
            c_EL.CameraElement.rIRect.Width = 64;
            c_EL.CameraElement.rIRect.Height = 64;
            c_EL.CameraElement.sImageSize = new Size(64, 64);
            c_EL.CameraElement.bBrush = false;
            c_EL.CameraElement.nStringIndiceSize = 1;

            c_EL.LeverElement = new CElementEvent();
            c_EL.LeverElement.szEvent = "Lever_";
            c_EL.LeverElement.nImageID = mTM.LoadTexture("..\\..\\Resources\\Lever.png", Color.FromArgb(255, 255, 255, 255).ToArgb());
            c_EL.LeverElement.rIRect.X = 0;
            c_EL.LeverElement.rIRect.Y = 0;
            c_EL.LeverElement.rIRect.Width = 32;
            c_EL.LeverElement.rIRect.Height = 64;
            c_EL.LeverElement.sImageSize = c_EL.LeverElement.rIRect.Size;
            c_EL.LeverElement.bBrush = false;
            c_EL.LeverElement.nStringIndiceSize = 1;

            c_EL.SpringElement = new CElementEvent();
            c_EL.SpringElement.szEvent = "Spring_";
            c_EL.SpringElement.nImageID = -1;
            c_EL.SpringElement.rIRect.X = 0;
            c_EL.SpringElement.rIRect.Y = 0;
            c_EL.SpringElement.rIRect.Width = 32;
            c_EL.SpringElement.rIRect.Height = 32;
            c_EL.SpringElement.sImageSize = new Size(32, 32);
            c_EL.SpringElement.nLayerType = new int();
            c_EL.SpringElement.bBrush = false;
            c_EL.SpringElement.nStringIndiceSize = 1;

            c_EL.BalloonElement = new CElementEvent();
            c_EL.BalloonElement.szEvent = "Balloon_";
            c_EL.BalloonElement.nImageID = -1;
            c_EL.BalloonElement.rIRect.X = 0;
            c_EL.BalloonElement.rIRect.Y = 0;
            c_EL.BalloonElement.rIRect.Width = 32;
            c_EL.BalloonElement.rIRect.Height = 96;
            c_EL.BalloonElement.sImageSize = new Size(32, 96);
            c_EL.BalloonElement.bBrush = false;
            c_EL.BalloonElement.nStringIndiceSize = 1;

            c_EL.CrateElement = new CElementEvent();
            c_EL.CrateElement.szEvent = "Crate_";
            c_EL.CrateElement.nImageID = -1;
            c_EL.CrateElement.rIRect.X = 0;
            c_EL.CrateElement.rIRect.Y = 0;
            c_EL.CrateElement.rIRect.Width = 32;
            c_EL.CrateElement.rIRect.Height = 32;
            c_EL.CrateElement.sImageSize = new Size(32, 32);
            c_EL.CrateElement.bBrush = false;
            c_EL.CrateElement.nStringIndiceSize = 1;

            // Powerup Objects
            c_EL.PogoElement = new CElementEvent();
            c_EL.PogoElement.szEvent = "PU_Pogo_";
            c_EL.PogoElement.nPowerupType = (int)PowerupType.POGO;
            c_EL.PogoElement.nImageID = -1;
            c_EL.PogoElement.rIRect.X = 0;
            c_EL.PogoElement.rIRect.Y = 0;
            c_EL.PogoElement.rIRect.Width = 32;
            c_EL.PogoElement.rIRect.Height = 32;
            c_EL.PogoElement.sImageSize = new Size(32, 32);
            c_EL.PogoElement.bBrush = false;
            c_EL.PogoElement.nStringIndiceSize = 1;

            c_EL.CheetahElement = new CElementEvent();
            c_EL.CheetahElement.szEvent = "PU_Cheetah_";
            c_EL.CheetahElement.nPowerupType = (int)PowerupType.CHEETAH;
            c_EL.CheetahElement.nImageID = -1;
            c_EL.CheetahElement.rIRect.X = 0;
            c_EL.CheetahElement.rIRect.Y = 0;
            c_EL.CheetahElement.rIRect.Width = 32;
            c_EL.CheetahElement.rIRect.Height = 32;
            c_EL.CheetahElement.sImageSize = new Size(32, 32);
            c_EL.CheetahElement.bBrush = false;
            c_EL.CheetahElement.nStringIndiceSize = 1;

            c_EL.FizzyElement = new CElementEvent();
            c_EL.FizzyElement.szEvent = "PU_Fizzy_";
            c_EL.FizzyElement.nPowerupType = (int)PowerupType.FIZZY;
            c_EL.FizzyElement.nImageID = -1;
            c_EL.FizzyElement.rIRect.X = 0;
            c_EL.FizzyElement.rIRect.Y = 0;
            c_EL.FizzyElement.rIRect.Width = 32;
            c_EL.FizzyElement.rIRect.Height = 32;
            c_EL.FizzyElement.sImageSize = new Size(32, 32);
            c_EL.FizzyElement.bBrush = false;
            c_EL.FizzyElement.nStringIndiceSize = 1;

            c_EL.GreasedPigElement = new CElementEvent();
            c_EL.GreasedPigElement.szEvent = "PU_Greased_Pig_";
            c_EL.GreasedPigElement.nPowerupType = (int)PowerupType.GREASED_PIG;
            c_EL.GreasedPigElement.nImageID = -1;
            c_EL.GreasedPigElement.rIRect.X = 0;
            c_EL.GreasedPigElement.rIRect.Y = 0;
            c_EL.GreasedPigElement.rIRect.Width = 32;
            c_EL.GreasedPigElement.rIRect.Height = 32;
            c_EL.GreasedPigElement.sImageSize = new Size(32, 32);
            c_EL.GreasedPigElement.bBrush = false;
            c_EL.GreasedPigElement.nStringIndiceSize = 1;

            c_EL.FlubbElement = new CElementEvent();
            c_EL.FlubbElement.szEvent = "PU_Flubb_";
            c_EL.FlubbElement.nPowerupType = (int)PowerupType.FLUBBER;
            c_EL.FlubbElement.nImageID = -1;
            c_EL.FlubbElement.rIRect.X = 0;
            c_EL.FlubbElement.rIRect.Y = 0;
            c_EL.FlubbElement.rIRect.Width = 32;
            c_EL.FlubbElement.rIRect.Height = 32;
            c_EL.FlubbElement.sImageSize = new Size(32, 32);
            c_EL.FlubbElement.bBrush = false;
            c_EL.FlubbElement.nStringIndiceSize = 1;

            // Tools Objects
            c_EL.WaterGunElement = new CElementEvent();
            c_EL.WaterGunElement.szEvent = "Tool_Water_Gun_";
            c_EL.WaterGunElement.nPowerupType = (int)ToolType.SQUIRT;
            c_EL.WaterGunElement.nImageID = -1;
            c_EL.WaterGunElement.rIRect.X = 0;
            c_EL.WaterGunElement.rIRect.Y = 0;
            c_EL.WaterGunElement.rIRect.Width = 32;
            c_EL.WaterGunElement.rIRect.Height = 32;
            c_EL.WaterGunElement.sImageSize = new Size(32, 32);
            c_EL.WaterGunElement.bBrush = false;
            c_EL.WaterGunElement.nStringIndiceSize = 1;

            c_EL.CottonCandyElement = new CElementEvent();
            c_EL.CottonCandyElement.szEvent = "Tool_Cotton_Candy_Gun_";
            c_EL.CottonCandyElement.nPowerupType = (int)ToolType.COTTON_CANDY;
            c_EL.CottonCandyElement.nImageID = -1;
            c_EL.CottonCandyElement.rIRect.X = 0;
            c_EL.CottonCandyElement.rIRect.Y = 0;
            c_EL.CottonCandyElement.rIRect.Width = 64;
            c_EL.CottonCandyElement.rIRect.Height = 32;
            c_EL.CottonCandyElement.sImageSize = new Size(64, 32);
            c_EL.CottonCandyElement.bBrush = false;
            c_EL.CottonCandyElement.nStringIndiceSize = 1;

            c_EL.BalloonGunElement = new CElementEvent();
            c_EL.BalloonGunElement.szEvent = "Tool_Loon_Gun_";
            c_EL.BalloonGunElement.nPowerupType = (int)ToolType.BALLOON;
            c_EL.BalloonGunElement.nImageID = -1;
            c_EL.BalloonGunElement.rIRect.X = 0;
            c_EL.BalloonGunElement.rIRect.Y = 0;
            c_EL.BalloonGunElement.rIRect.Width = 64;
            c_EL.BalloonGunElement.rIRect.Height = 32;
            c_EL.BalloonGunElement.sImageSize = new Size(64, 32);
            c_EL.BalloonGunElement.bBrush = false;
            c_EL.BalloonGunElement.nStringIndiceSize = 1;

            // Enemy Objects
            c_EL.RatElement = new CElementEvent();
            c_EL.RatElement.szEvent = "Rat_";
            c_EL.RatElement.nImageID = -1;
            c_EL.RatElement.rIRect.X = 0;
            c_EL.RatElement.rIRect.Y = 0;
            c_EL.RatElement.rIRect.Width = 32;
            c_EL.RatElement.rIRect.Height = 32;
            c_EL.RatElement.sImageSize = new Size(32, 32);
            c_EL.RatElement.bBrush = false;
            c_EL.RatElement.nStringIndiceSize = 1;
            c_EL.RatElement.nPatrolDistance = 7;

            c_EL.ClownElement = new CElementEvent();
            c_EL.ClownElement.szEvent = "Clown_";
            c_EL.ClownElement.nImageID = -1;
            c_EL.ClownElement.rIRect.X = 0;
            c_EL.ClownElement.rIRect.Y = 0;
            c_EL.ClownElement.rIRect.Width = 32;
            c_EL.ClownElement.rIRect.Height = 64;
            c_EL.ClownElement.sImageSize = new Size(32, 64);
            c_EL.ClownElement.bBrush = false;
            c_EL.ClownElement.nStringIndiceSize = 1;
            c_EL.ClownElement.nPatrolDistance = 7;

            c_EL.HoodlumElement = new CElementEvent();
            c_EL.HoodlumElement.szEvent = "Hoodlum_";
            c_EL.HoodlumElement.nImageID = -1;
            c_EL.HoodlumElement.rIRect.X = 0;
            c_EL.HoodlumElement.rIRect.Y = 0;
            c_EL.HoodlumElement.rIRect.Width = 32;
            c_EL.HoodlumElement.rIRect.Height = 64;
            c_EL.HoodlumElement.sImageSize = new Size(32, 64);
            c_EL.HoodlumElement.bBrush = false;
            c_EL.HoodlumElement.nStringIndiceSize = 1;

            // Characters
            c_EL.CorinneElement = new CElementEvent();
            c_EL.CorinneElement.szEvent = "Corinne_Player";
            c_EL.CorinneElement.nImageID = -1;
            c_EL.CorinneElement.rIRect.X = 0;
            c_EL.CorinneElement.rIRect.Y = 0;
            c_EL.CorinneElement.rIRect.Width = 32;
            c_EL.CorinneElement.rIRect.Height = 64;
            c_EL.CorinneElement.sImageSize = new Size(32, 64);
            c_EL.CorinneElement.bBrush = false;
            c_EL.CorinneElement.nStringIndiceSize = 1;

            c_EL.FoxElement = new CElementEvent();
            c_EL.FoxElement.szEvent = "Fox_Player";
            c_EL.FoxElement.nImageID = -1;
            c_EL.FoxElement.rIRect.X = 0;
            c_EL.FoxElement.rIRect.Y = 0;
            c_EL.FoxElement.rIRect.Width = 64;
            c_EL.FoxElement.rIRect.Height = 96;
            c_EL.FoxElement.sImageSize = new Size(64, 96);
            c_EL.FoxElement.bBrush = false;
            c_EL.FoxElement.nStringIndiceSize = 1;

            c_EL.CheckpointElement = new CElementEvent();
            c_EL.CheckpointElement.szEvent = "Checkpoint_";
            c_EL.CheckpointElement.nImageID = -1;
            c_EL.CheckpointElement.rIRect.X = 0;
            c_EL.CheckpointElement.rIRect.Y = 0;
            c_EL.CheckpointElement.rIRect.Width = 32;
            c_EL.CheckpointElement.rIRect.Height = 64;
            c_EL.CheckpointElement.sImageSize = new Size(32, 64);
            c_EL.CheckpointElement.bBrush = false;
            c_EL.CheckpointElement.nStringIndiceSize = 1;
        }

        // Render All //////////////////////////////////////////////////////////////////////////////////
        public void Render()
        {

            mD3D.Clear(Tile_World_Panel, Color.FromArgb(255, 100, 100, 100));

            mD3D.DeviceBegin();
            mD3D.SpriteBegin();


            if (bBackLayer != false)
                RenderBackLayer();

            if (bPlayLayer != false || bMidLayer != false || bFrontLayer != false)
                RenderTileWorld();

            if (bPlayLayer != false )
                RenderCollisionLayer();

            if (bPlayLayer != false)
                RenderElementLayer();

            if (bBeginLayer != false)
                mD3D.DrawText("Select Layer to Begin", Tile_World_Panel.Width / 2 - 75, Tile_World_Panel.Height / 2 + 8, Color.Yellow);

            RenderTileWindowGrid();
            //mD3D.Sprite.Flush();

            

            mD3D.SpriteEnd();
            mD3D.DeviceEnd();

            mD3D.Present(Tile_World_Panel);

            mD3D.Clear(Tileset_Panel, Color.FromArgb(255, 150, 150, 150));

            mD3D.DeviceBegin();
            mD3D.SpriteBegin();

            RenderTileSet();

            mD3D.SpriteEnd();
            mD3D.DeviceEnd();

            mD3D.Present(Tileset_Panel);

        
        }

        // Background Layer Render /////////////////////////////////////////////////////////////////////
        private void RenderBackLayer()
        {
            mD3D.Sprite.Flush();

            Rectangle destRect = Rectangle.Empty;
            destRect.X = c_EL.BackgroundIMG.rIRect.X - Tile_World_Panel.AutoScrollPosition.X;
            destRect.Y = c_EL.BackgroundIMG.rIRect.Y - Tile_World_Panel.AutoScrollPosition.Y;
            destRect.Size = new Size(800, 600);

            mTM.Draw(c_EL.BackgroundIMG.nImageID, 0, 0, 1.0f, 1.0f, destRect, 0, 0, 0.0f, 0);
        }

        // World Grid Render ///////////////////////////////////////////////////////////////////////////
        private void RenderTileWindowGrid()
        {
            // Drawing Grid on the World
            for (int x = 0; x < mapSize.Width; x++)
            {
                for (int y = 0; y < mapSize.Height; y++)
                {
                    // Tile Size converting to Destination Rect
                    Rectangle destRect = Rectangle.Empty;
                    destRect.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                    destRect.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                    destRect.Size = tileSize;

                    if (x + tileSize.Width > (Tile_World_Panel.AutoScrollPosition.X / 32) &&
                             x < Tile_World_Panel.AutoScrollPosition.X / 32 + Tile_World_Panel.Width)
                        mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.DarkGray), 0.25f);

                }
            }
        }

        // World Render ////////////////////////////////////////////////////////////////////////////////
        private void RenderTileWorld()
        {
            mD3D.Sprite.Flush();

            if (bPlayLayer != false || bMidLayer != false || bFrontLayer != false)
            {
                // Drawing Grid on the Tileset
                for (int x = 0; x < mapSize.Width; x++)
                {
                    for (int y = 0; y < mapSize.Height; y++)
                    {
                        // Tile Size converting to Destination Rect
                        Rectangle destRect = Rectangle.Empty;
                        destRect.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect.Size = tileSize;

                        c_TM.rPlayRect[x, y].Size = tileSize;

                        Rectangle destRect2 = Rectangle.Empty;
                        destRect2.X = selectedWorldDrag.X + Tile_World_Panel.AutoScrollPosition.X / tileSize.Width;
                        destRect2.Y = selectedWorldDrag.Y + Tile_World_Panel.AutoScrollPosition.Y / tileSize.Height;
                        destRect2.Size = new Size(selectedEndTile.X, selectedEndTile.Y);

                        if ( x + tileSize.Width > (Tile_World_Panel.AutoScrollPosition.X / 32 ) &&
                             x < Tile_World_Panel.AutoScrollPosition.X / 32 + Tile_World_Panel.Width )
                        {

                            if (c_TM.nMidBackTileImageID[x, y] != -1)
                            {
                                if (bMidLayer != false)
                                    mTM.Draw(c_TM.nMidBackTileImageID[x, y], destRect.X, destRect.Y, 1.0f, 1.0f, c_TM.rMidBackRect[x, y], 0, 0, 0.0f, 0);
                            }

                            if (c_TM.nPlayTileImageID[x, y] != -1)
                            {
                                if (bPlayLayer != false)
                                    mTM.Draw(c_TM.nPlayTileImageID[x, y], destRect.X, destRect.Y, 1.0f, 1.0f, c_TM.rPlayRect[x, y], 0, 0, 0.0f, 0);
                            }

                            if (c_TM.nFrontTileImageID[x, y] != -1)
                            {
                                if (bFrontLayer != false)
                                    mTM.Draw(c_TM.nFrontTileImageID[x, y], destRect.X, destRect.Y, 1.0f, 1.0f, c_TM.rFrontRect[x, y], 0, 0, 0.0f, 0);
                            }

                            // Render code for "Indicator at Mouse point"
                            if (bIsTileOutsideMap != true)
                            {
                                //mD3D.DrawRect(new Rectangle(selectedWorldDrag, new Size(selectedEndTile.X, selectedEndTile.Y)), Color.LightSteelBlue);
                                //mD3D.DrawRect(destRect2, Color.FromName("Red"));
                                //mD3D.DrawEmptyRect(destRect2, Color.FromKnownColor(KnownColor.Firebrick), 3.00f);
                                //mD3D.DrawLine(destRect2.X, destRect2.Y, destRect2.X - 64, destRect2.Y, Color.FromKnownColor(KnownColor.WhiteSmoke), 0.5f);
                                //mD3D.DrawLine(destRect2.X, destRect2.Y + 32, destRect2.X - 64, destRect2.Y + 32, Color.FromKnownColor(KnownColor.WhiteSmoke), 0.5f);
                                //mD3D.DrawLine(destRect2.X + 32, destRect2.Y, destRect2.X + 96, destRect2.Y, Color.FromKnownColor(KnownColor.WhiteSmoke), 0.5f);
                                //mD3D.DrawLine(destRect2.X + 32, destRect2.Y + 32, destRect2.X + 96, destRect2.Y + 32, Color.FromKnownColor(KnownColor.WhiteSmoke), 0.5f);
                            }
                            else
                            {
                                //mD3D.DrawRect(new Rectangle(selectedWorldDrag, new Size(selectedEndTile.X, selectedEndTile.Y)), Color.Maroon);

                                mD3D.DrawText("You are outside the bounds.", destRect2.Left, destRect2.Top, Color.WhiteSmoke);
                                mD3D.DrawRect(destRect2, Color.Maroon);
                                //mD3D.DrawEmptyRect(destRect2, Color.FromKnownColor(KnownColor.Maroon), 3.00f);
                            }

                            //mD3D.Sprite.Flush();

                            if (c_TM.nCollideType[x, y] == (int)CollideType.JUMP)
                            {
                                mD3D.Sprite.Flush();
                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.Orange), 3.00f);
                                mD3D.DrawText(" J", destRect.X, destRect.Y, Color.FromKnownColor(KnownColor.Orange));
                            }

                            if (c_TM.nCollideType[x, y] == (int)CollideType.STOP)
                            {
                                mD3D.Sprite.Flush();
                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.Orange), 3.00f);
                                mD3D.DrawText(" S", destRect.X, destRect.Y, Color.FromKnownColor(KnownColor.Orange));
                            }

                            if (c_TM.nCollideType[x, y] == (int)CollideType.GLASS)
                            {
                                mD3D.Sprite.Flush();
                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.LightBlue), 2.00f);
                                mD3D.DrawText(" G", destRect.X, destRect.Y, Color.FromKnownColor(KnownColor.LightBlue));
                            }

                            if (c_TM.nCollideType[x, y] == (int)CollideType.GRAVITYON)
                            {
                                mD3D.Sprite.Flush();
                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.WhiteSmoke), 2.00f);
                                mD3D.DrawText(" GR", destRect.X, destRect.Y, Color.FromKnownColor(KnownColor.WhiteSmoke));
                            }

                            if (bIsTileOutsideMap == true)
                            {
                                //mD3D.Sprite.Flush();
                                mD3D.DrawEmptyRect(destRect2, Color.FromKnownColor(KnownColor.Brown), 5.00f);

                            }

                            //mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.DarkGray), 0.25f);
                        }

                        
                        
                    }
                }
            }



        }

        // Collision Layer Rendering ///////////////////////////////////////////////////////////////////
        private void RenderCollisionLayer()
        {
            mD3D.Sprite.Flush();
            if(bCollisionBrush != false)
            {
                // Drawing Collision Rectangles
                for (int x = 0; x < mapSize.Width; x++)
                {
                    for (int y = 0; y < mapSize.Height; y++)
                    {
                        // Tile Size converting to Destination Rect
                        Rectangle destRect = Rectangle.Empty;
                        destRect.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect.Size = tileSize;

                        if (x + tileSize.Width > (Tile_World_Panel.AutoScrollPosition.X / 32) &&
                             x < Tile_World_Panel.AutoScrollPosition.X / 32 + Tile_World_Panel.Width)
                        {
                            if (c_TM.nCollideType[x, y] == (int)CollideType.COLLIDE)
                            {
                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.Red), 2.00f);

                                mD3D.DrawLine(destRect.X, destRect.Y,
                                    destRect.Width + destRect.X, destRect.Height + destRect.Y,
                                    Color.FromKnownColor(KnownColor.Red), 1.25f);

                                mD3D.DrawText("C", destRect.X, destRect.Y, Color.FromKnownColor(KnownColor.DarkRed));
                            }
                        }
                    }
                }
            }
        }

        // Element Layer Render ////////////////////////////////////////////////////////////////////////
        private void RenderElementLayer()
        {
            mD3D.Sprite.Flush();
            for (int x = 0; x < mapSize.Width; x++)
            {
                for (int y = 0; y < mapSize.Height; y++)
                {

                    if (x + tileSize.Width > (Tile_World_Panel.AutoScrollPosition.X / 32) &&
                             x < Tile_World_Panel.AutoScrollPosition.X / 32 + Tile_World_Panel.Width)
                    {
                        // Horizontal Gate
                        Rectangle destRect = Rectangle.Empty;
                        destRect.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect.Size = c_EL.HorizontalGateElement.sImageSize;

                        c_EL.HorizontalGateElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.HorizontalGateElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.HorizontalGateElement.rIRect.Size = c_EL.HorizontalGateElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null)
                        {
                            if (c_TM.szEventID[x, y].StartsWith("Horz_Gate_"))
                            {
                                //mTM.Draw(c_EL.HorizontalGateElement.nImageID, destRect.X, destRect.Y, 1.0f, 1.0f, c_EL.HorizontalGateElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                                mD3D.DrawEmptyRect(destRect, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                                mD3D.DrawLine(destRect.X + c_EL.HorizontalGateElement.sImageSize.Width / 2 - 3,
                                    destRect.Y + c_EL.HorizontalGateElement.sImageSize.Height / 2,
                                    destRect.X + c_EL.HorizontalGateElement.sImageSize.Width / 2 + 4,
                                    destRect.Y + c_EL.HorizontalGateElement.sImageSize.Height / 2,
                                    Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                                mD3D.DrawLine(destRect.X + c_EL.HorizontalGateElement.sImageSize.Width / 2,
                                    destRect.Y + c_EL.HorizontalGateElement.sImageSize.Height / 2 - 4,
                                    destRect.X + c_EL.HorizontalGateElement.sImageSize.Width / 2,
                                    destRect.Y + c_EL.HorizontalGateElement.sImageSize.Height / 2 + 4,
                                    Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            }
                        }

                        // Vertical Gate
                        Rectangle dest23Rect = Rectangle.Empty;
                        dest23Rect.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        dest23Rect.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        dest23Rect.Size = c_EL.VerticalGateElement.sImageSize;

                        c_EL.VerticalGateElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.VerticalGateElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.VerticalGateElement.rIRect.Size = c_EL.VerticalGateElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null)
                        {
                            if (c_TM.szEventID[x, y].StartsWith("Vert_Gate_"))
                            {
                                //mTM.Draw(c_EL.HorizontalGateElement.nImageID, destRect.X, destRect.Y, 1.0f, 1.0f, c_EL.HorizontalGateElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                                mD3D.DrawEmptyRect(dest23Rect, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                                mD3D.DrawLine(dest23Rect.X + c_EL.VerticalGateElement.sImageSize.Width / 2 - 3,
                                    dest23Rect.Y + c_EL.VerticalGateElement.sImageSize.Height / 2,
                                    dest23Rect.X + c_EL.VerticalGateElement.sImageSize.Width / 2 + 4,
                                    dest23Rect.Y + c_EL.VerticalGateElement.sImageSize.Height / 2,
                                    Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                                mD3D.DrawLine(dest23Rect.X + c_EL.VerticalGateElement.sImageSize.Width / 2,
                                    dest23Rect.Y + c_EL.VerticalGateElement.sImageSize.Height / 2 - 4,
                                    dest23Rect.X + c_EL.VerticalGateElement.sImageSize.Width / 2,
                                    dest23Rect.Y + c_EL.VerticalGateElement.sImageSize.Height / 2 + 4,
                                    Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            }
                        }

                        // Pressure Plate
                        Rectangle destRect2 = Rectangle.Empty;
                        destRect2.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect2.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect2.Size = c_EL.PressurePlateElement.sImageSize;

                        c_EL.PressurePlateElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.PressurePlateElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.PressurePlateElement.rIRect.Size = c_EL.PressurePlateElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Pressure_Plate_"))
                        {
                            //mTM.Draw(c_EL.PressurePlateElement.nImageID, destRect2.X, destRect2.Y, 1.0f, 1.0f, c_EL.PressurePlateElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                            mD3D.DrawEmptyRect(destRect2, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect2.X + c_EL.PressurePlateElement.sImageSize.Width / 2 - 3,
                                destRect2.Y + c_EL.PressurePlateElement.sImageSize.Height / 2,
                                destRect2.X + c_EL.PressurePlateElement.sImageSize.Width / 2 + 4,
                                destRect2.Y + c_EL.PressurePlateElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect2.X + c_EL.PressurePlateElement.sImageSize.Width / 2,
                                destRect2.Y + c_EL.PressurePlateElement.sImageSize.Height / 2 - 4,
                                destRect2.X + c_EL.PressurePlateElement.sImageSize.Width / 2,
                                destRect2.Y + c_EL.PressurePlateElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Camera
                        Rectangle destRect3 = Rectangle.Empty;
                        destRect3.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect3.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect3.Size = c_EL.CameraElement.sImageSize;

                        c_EL.CameraElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CameraElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CameraElement.rIRect.Size = c_EL.CameraElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Security_Cam_"))
                        {
                            //mTM.Draw(c_EL.CameraElement.nImageID, destRect3.X, destRect3.Y, 1.0f, 1.0f, c_EL.CameraElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                            mD3D.DrawEmptyRect(destRect3, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect3.X + c_EL.CameraElement.sImageSize.Width / 2 - 3,
                                destRect3.Y + c_EL.CameraElement.sImageSize.Height / 2,
                                destRect3.X + c_EL.CameraElement.sImageSize.Width / 2 + 4,
                                destRect3.Y + c_EL.CameraElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect3.X + c_EL.CameraElement.sImageSize.Width / 2,
                                destRect3.Y + c_EL.CameraElement.sImageSize.Height / 2 - 4,
                                destRect3.X + c_EL.CameraElement.sImageSize.Width / 2,
                                destRect3.Y + c_EL.CameraElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Lever
                        Rectangle destRect4 = Rectangle.Empty;
                        destRect4.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect4.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect4.Size = c_EL.LeverElement.sImageSize;

                        c_EL.LeverElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.LeverElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.LeverElement.rIRect.Size = c_EL.LeverElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Lever_"))
                        {
                            //mTM.Draw(c_EL.LeverElement.nImageID, destRect4.X, destRect4.Y + 14, 1.0f, 1.0f, c_EL.LeverElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                            mD3D.DrawEmptyRect(destRect4, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect4.X + c_EL.LeverElement.sImageSize.Width / 2 - 3,
                                destRect4.Y + c_EL.LeverElement.sImageSize.Height / 2,
                                destRect4.X + c_EL.LeverElement.sImageSize.Width / 2 + 4,
                                destRect4.Y + c_EL.LeverElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect4.X + c_EL.LeverElement.sImageSize.Width / 2,
                                destRect4.Y + c_EL.LeverElement.sImageSize.Height / 2 - 4,
                                destRect4.X + c_EL.LeverElement.sImageSize.Width / 2,
                                destRect4.Y + c_EL.LeverElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Spring
                        Rectangle destRect5 = Rectangle.Empty;
                        destRect5.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect5.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect5.Size = c_EL.SpringElement.sImageSize;

                        c_EL.SpringElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.SpringElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.SpringElement.rIRect.Size = c_EL.SpringElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Spring_"))
                        {
                            //mTM.Draw(c_EL.SpringElement.nImageID, destRect5.X, destRect5.Y, 1.0f, 1.0f, c_EL.LeverElement.rIRect, 0, 0, 0.0f, Color.FromKnownColor(KnownColor.White));

                            mD3D.DrawEmptyRect(destRect5, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect5.X + c_EL.SpringElement.sImageSize.Width / 2 - 3,
                                destRect5.Y + c_EL.SpringElement.sImageSize.Height / 2,
                                destRect5.X + c_EL.SpringElement.sImageSize.Width / 2 + 4,
                                destRect5.Y + c_EL.SpringElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect5.X + c_EL.SpringElement.sImageSize.Width / 2,
                                destRect5.Y + c_EL.SpringElement.sImageSize.Height / 2 - 4,
                                destRect5.X + c_EL.SpringElement.sImageSize.Width / 2,
                                destRect5.Y + c_EL.SpringElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Balloon
                        Rectangle destRect6 = Rectangle.Empty;
                        destRect6.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect6.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect6.Size = c_EL.BalloonElement.sImageSize;

                        c_EL.BalloonElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.BalloonElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.BalloonElement.rIRect.Size = c_EL.BalloonElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Balloon_"))
                        {
                            mD3D.DrawEmptyRect(destRect6, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect6.X + c_EL.BalloonElement.sImageSize.Width / 2 - 3,
                                destRect6.Y + c_EL.BalloonElement.sImageSize.Height / 2,
                                destRect6.X + c_EL.BalloonElement.sImageSize.Width / 2 + 4,
                                destRect6.Y + c_EL.BalloonElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect6.X + c_EL.BalloonElement.sImageSize.Width / 2,
                                destRect6.Y + c_EL.BalloonElement.sImageSize.Height / 2 - 4,
                                destRect6.X + c_EL.BalloonElement.sImageSize.Width / 2,
                                destRect6.Y + c_EL.BalloonElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Crate
                        Rectangle destRect23 = Rectangle.Empty;
                        destRect23.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect23.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect23.Size = c_EL.CrateElement.sImageSize;

                        c_EL.CrateElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CrateElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CrateElement.rIRect.Size = c_EL.CrateElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Crate_"))
                        {
                            mD3D.DrawEmptyRect(destRect23, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect23.X + c_EL.CrateElement.sImageSize.Width / 2 - 3,
                                destRect23.Y + c_EL.CrateElement.sImageSize.Height / 2,
                                destRect23.X + c_EL.CrateElement.sImageSize.Width / 2 + 4,
                                destRect23.Y + c_EL.CrateElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect23.X + c_EL.CrateElement.sImageSize.Width / 2,
                                destRect23.Y + c_EL.CrateElement.sImageSize.Height / 2 - 4,
                                destRect23.X + c_EL.CrateElement.sImageSize.Width / 2,
                                destRect23.Y + c_EL.CrateElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Pogo Powerup
                        Rectangle destRect7 = Rectangle.Empty;
                        destRect7.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect7.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect7.Size = c_EL.PogoElement.sImageSize;

                        c_EL.PogoElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.PogoElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.PogoElement.rIRect.Size = c_EL.PogoElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Pogo_"))
                        {
                            mD3D.DrawEmptyRect(destRect7, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect7.X + c_EL.PogoElement.sImageSize.Width / 2 - 3,
                                destRect7.Y + c_EL.PogoElement.sImageSize.Height / 2,
                                destRect7.X + c_EL.PogoElement.sImageSize.Width / 2 + 4,
                                destRect7.Y + c_EL.PogoElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect7.X + c_EL.PogoElement.sImageSize.Width / 2,
                                destRect7.Y + c_EL.PogoElement.sImageSize.Height / 2 - 4,
                                destRect7.X + c_EL.PogoElement.sImageSize.Width / 2,
                                destRect7.Y + c_EL.PogoElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Cheetah Powerup
                        Rectangle destRect8 = Rectangle.Empty;
                        destRect8.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect8.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect8.Size = c_EL.CheetahElement.sImageSize;

                        c_EL.CheetahElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CheetahElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CheetahElement.rIRect.Size = c_EL.CheetahElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Cheetah_"))
                        {
                            mD3D.DrawEmptyRect(destRect8, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect8.X + c_EL.CheetahElement.sImageSize.Width / 2 - 3,
                                destRect8.Y + c_EL.CheetahElement.sImageSize.Height / 2,
                                destRect8.X + c_EL.CheetahElement.sImageSize.Width / 2 + 4,
                                destRect8.Y + c_EL.CheetahElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect8.X + c_EL.CheetahElement.sImageSize.Width / 2,
                                destRect8.Y + c_EL.CheetahElement.sImageSize.Height / 2 - 4,
                                destRect8.X + c_EL.CheetahElement.sImageSize.Width / 2,
                                destRect8.Y + c_EL.CheetahElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Fizzy Powerup
                        Rectangle destRect9 = Rectangle.Empty;
                        destRect9.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect9.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect9.Size = c_EL.FizzyElement.sImageSize;

                        c_EL.FizzyElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.FizzyElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.FizzyElement.rIRect.Size = c_EL.FizzyElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Fizzy_"))
                        {
                            mD3D.DrawEmptyRect(destRect9, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect9.X + c_EL.FizzyElement.sImageSize.Width / 2 - 3,
                                destRect9.Y + c_EL.FizzyElement.sImageSize.Height / 2,
                                destRect9.X + c_EL.FizzyElement.sImageSize.Width / 2 + 4,
                                destRect9.Y + c_EL.FizzyElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect9.X + c_EL.FizzyElement.sImageSize.Width / 2,
                                destRect9.Y + c_EL.FizzyElement.sImageSize.Height / 2 - 4,
                                destRect9.X + c_EL.FizzyElement.sImageSize.Width / 2,
                                destRect9.Y + c_EL.FizzyElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Greased Pig
                        Rectangle destRect10 = Rectangle.Empty;
                        destRect10.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect10.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect10.Size = c_EL.GreasedPigElement.sImageSize;

                        c_EL.GreasedPigElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.GreasedPigElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.GreasedPigElement.rIRect.Size = c_EL.HorizontalGateElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Greased_Pig_"))
                        {
                            mD3D.DrawEmptyRect(destRect10, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect10.X + c_EL.GreasedPigElement.sImageSize.Width / 2 - 3,
                                destRect10.Y + c_EL.GreasedPigElement.sImageSize.Height / 2,
                                destRect10.X + c_EL.GreasedPigElement.sImageSize.Width / 2 + 4,
                                destRect10.Y + c_EL.GreasedPigElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect10.X + c_EL.GreasedPigElement.sImageSize.Width / 2,
                                destRect10.Y + c_EL.GreasedPigElement.sImageSize.Height / 2 - 4,
                                destRect10.X + c_EL.GreasedPigElement.sImageSize.Width / 2,
                                destRect10.Y + c_EL.GreasedPigElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Flubb
                        Rectangle destRect11 = Rectangle.Empty;
                        destRect11.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect11.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect11.Size = c_EL.FlubbElement.sImageSize;

                        c_EL.FlubbElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.FlubbElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.FlubbElement.rIRect.Size = c_EL.FlubbElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Flubb_"))
                        {
                            mD3D.DrawEmptyRect(destRect11, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect11.X + c_EL.FlubbElement.sImageSize.Width / 2 - 3,
                                destRect11.Y + c_EL.FlubbElement.sImageSize.Height / 2,
                                destRect11.X + c_EL.FlubbElement.sImageSize.Width / 2 + 4,
                                destRect11.Y + c_EL.FlubbElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect11.X + c_EL.FlubbElement.sImageSize.Width / 2,
                                destRect11.Y + c_EL.FlubbElement.sImageSize.Height / 2 - 4,
                                destRect11.X + c_EL.FlubbElement.sImageSize.Width / 2,
                                destRect11.Y + c_EL.FlubbElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Water Gun
                        Rectangle destRect12 = Rectangle.Empty;
                        destRect12.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect12.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect12.Size = c_EL.WaterGunElement.sImageSize;

                        c_EL.WaterGunElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.WaterGunElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.WaterGunElement.rIRect.Size = c_EL.WaterGunElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Water_Gun_"))
                        {
                            mD3D.DrawEmptyRect(destRect12, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect12.X + c_EL.WaterGunElement.sImageSize.Width / 2 - 3,
                                destRect12.Y + c_EL.WaterGunElement.sImageSize.Height / 2,
                                destRect12.X + c_EL.WaterGunElement.sImageSize.Width / 2 + 4,
                                destRect12.Y + c_EL.WaterGunElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect12.X + c_EL.WaterGunElement.sImageSize.Width / 2,
                                destRect12.Y + c_EL.WaterGunElement.sImageSize.Height / 2 - 4,
                                destRect12.X + c_EL.WaterGunElement.sImageSize.Width / 2,
                                destRect12.Y + c_EL.WaterGunElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Cotton Candy Gun
                        Rectangle destRect13 = Rectangle.Empty;
                        destRect13.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect13.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect13.Size = c_EL.CottonCandyElement.sImageSize;

                        c_EL.CottonCandyElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CottonCandyElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CottonCandyElement.rIRect.Size = c_EL.CottonCandyElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Cotton_Candy_Gun_"))
                        {
                            mD3D.DrawEmptyRect(destRect13, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect13.X + c_EL.CottonCandyElement.sImageSize.Width / 2 - 3,
                                destRect13.Y + c_EL.CottonCandyElement.sImageSize.Height / 2,
                                destRect13.X + c_EL.CottonCandyElement.sImageSize.Width / 2 + 4,
                                destRect13.Y + c_EL.CottonCandyElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect13.X + c_EL.CottonCandyElement.sImageSize.Width / 2,
                                destRect13.Y + c_EL.CottonCandyElement.sImageSize.Height / 2 - 4,
                                destRect13.X + c_EL.CottonCandyElement.sImageSize.Width / 2,
                                destRect13.Y + c_EL.CottonCandyElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Balloon Gun
                        Rectangle destRect14 = Rectangle.Empty;
                        destRect14.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect14.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect14.Size = c_EL.BalloonGunElement.sImageSize;

                        c_EL.BalloonGunElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.BalloonGunElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.BalloonGunElement.rIRect.Size = c_EL.BalloonGunElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Contains("Loon_Gun_"))
                        {
                            mD3D.DrawEmptyRect(destRect14, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect14.X + c_EL.BalloonGunElement.sImageSize.Width / 2 - 3,
                                destRect14.Y + c_EL.BalloonGunElement.sImageSize.Height / 2,
                                destRect14.X + c_EL.BalloonGunElement.sImageSize.Width / 2 + 4,
                                destRect14.Y + c_EL.BalloonGunElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect14.X + c_EL.BalloonGunElement.sImageSize.Width / 2,
                                destRect14.Y + c_EL.BalloonGunElement.sImageSize.Height / 2 - 4,
                                destRect14.X + c_EL.BalloonGunElement.sImageSize.Width / 2,
                                destRect14.Y + c_EL.BalloonGunElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Rat 
                        Rectangle destRect15 = Rectangle.Empty;
                        destRect15.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect15.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect15.Size = c_EL.RatElement.sImageSize;

                        c_EL.RatElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.RatElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.RatElement.rIRect.Size = c_EL.RatElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Rat_"))
                        {
                            mD3D.DrawEmptyRect(destRect15, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect15.X + c_EL.RatElement.sImageSize.Width / 2 - 3,
                                destRect15.Y + c_EL.RatElement.sImageSize.Height / 2,
                                destRect15.X + c_EL.RatElement.sImageSize.Width / 2 + 4,
                                destRect15.Y + c_EL.RatElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect15.X + c_EL.RatElement.sImageSize.Width / 2,
                                destRect15.Y + c_EL.RatElement.sImageSize.Height / 2 - 4,
                                destRect15.X + c_EL.RatElement.sImageSize.Width / 2,
                                destRect15.Y + c_EL.RatElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Clown
                        Rectangle destRect16 = Rectangle.Empty;
                        destRect16.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect16.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect16.Size = c_EL.ClownElement.sImageSize;

                        c_EL.ClownElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.ClownElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.ClownElement.rIRect.Size = c_EL.ClownElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Clown_"))
                        {
                            mD3D.DrawEmptyRect(destRect16, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect16.X + c_EL.ClownElement.sImageSize.Width / 2 - 3,
                                destRect16.Y + c_EL.ClownElement.sImageSize.Height / 2,
                                destRect16.X + c_EL.ClownElement.sImageSize.Width / 2 + 4,
                                destRect16.Y + c_EL.ClownElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect16.X + c_EL.ClownElement.sImageSize.Width / 2,
                                destRect16.Y + c_EL.ClownElement.sImageSize.Height / 2 - 4,
                                destRect16.X + c_EL.ClownElement.sImageSize.Width / 2,
                                destRect16.Y + c_EL.ClownElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Hoodlum
                        Rectangle destRect17 = Rectangle.Empty;
                        destRect17.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect17.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect17.Size = c_EL.HoodlumElement.sImageSize;

                        c_EL.HoodlumElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.HoodlumElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.HoodlumElement.rIRect.Size = c_EL.HoodlumElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Hoodlum_"))
                        {
                            mD3D.DrawEmptyRect(destRect17, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect17.X + c_EL.HoodlumElement.sImageSize.Width / 2 - 3,
                                destRect17.Y + c_EL.HoodlumElement.sImageSize.Height / 2,
                                destRect17.X + c_EL.HoodlumElement.sImageSize.Width / 2 + 4,
                                destRect17.Y + c_EL.HoodlumElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect17.X + c_EL.HoodlumElement.sImageSize.Width / 2,
                                destRect17.Y + c_EL.HoodlumElement.sImageSize.Height / 2 - 4,
                                destRect17.X + c_EL.HoodlumElement.sImageSize.Width / 2,
                                destRect17.Y + c_EL.HoodlumElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Checkpoint
                        Rectangle destRect18 = Rectangle.Empty;
                        destRect18.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect18.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect18.Size = c_EL.CheckpointElement.sImageSize;

                        c_EL.CheckpointElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CheckpointElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CheckpointElement.rIRect.Size = c_EL.CheckpointElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].StartsWith("Checkpoint_"))
                        {
                            mD3D.DrawEmptyRect(destRect18, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect18.X + c_EL.CheckpointElement.sImageSize.Width / 2 - 3,
                                destRect18.Y + c_EL.CheckpointElement.sImageSize.Height / 2,
                                destRect18.X + c_EL.CheckpointElement.sImageSize.Width / 2 + 4,
                                destRect18.Y + c_EL.CheckpointElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect18.X + c_EL.CheckpointElement.sImageSize.Width / 2,
                                destRect18.Y + c_EL.CheckpointElement.sImageSize.Height / 2 - 4,
                                destRect18.X + c_EL.CheckpointElement.sImageSize.Width / 2,
                                destRect18.Y + c_EL.CheckpointElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Corinne
                        Rectangle destRect20 = Rectangle.Empty;
                        destRect20.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect20.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect20.Size = c_EL.CorinneElement.sImageSize;

                        c_EL.CorinneElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.CorinneElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.CorinneElement.rIRect.Size = c_EL.CorinneElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Equals("Corinne_Player"))
                        {
                            mD3D.DrawEmptyRect(destRect20, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect20.X + c_EL.CorinneElement.sImageSize.Width / 2 - 3,
                                destRect20.Y + c_EL.CorinneElement.sImageSize.Height / 2,
                                destRect20.X + c_EL.CorinneElement.sImageSize.Width / 2 + 4,
                                destRect20.Y + c_EL.CorinneElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect20.X + c_EL.CorinneElement.sImageSize.Width / 2,
                                destRect20.Y + c_EL.CorinneElement.sImageSize.Height / 2 - 4,
                                destRect20.X + c_EL.CorinneElement.sImageSize.Width / 2,
                                destRect20.Y + c_EL.CorinneElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Fox
                        Rectangle destRect21 = Rectangle.Empty;
                        destRect21.X = x * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect21.Y = y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect21.Size = c_EL.FoxElement.sImageSize;

                        c_EL.FoxElement.rIRect.X = c_TM.map[x, y].X;
                        c_EL.FoxElement.rIRect.Y = c_TM.map[x, y].Y;
                        c_EL.FoxElement.rIRect.Size = c_EL.FoxElement.sImageSize;

                        if (c_TM.szEventID[x, y] != null && c_TM.szEventID[x, y].Equals("Fox_Player"))
                        {
                            mD3D.DrawEmptyRect(destRect21, Color.FromKnownColor(KnownColor.Yellow), 1.25f);
                            mD3D.DrawLine(destRect21.X + c_EL.FoxElement.sImageSize.Width / 2 - 3,
                                destRect21.Y + c_EL.FoxElement.sImageSize.Height / 2,
                                destRect21.X + c_EL.FoxElement.sImageSize.Width / 2 + 4,
                                destRect21.Y + c_EL.FoxElement.sImageSize.Height / 2,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                            mD3D.DrawLine(destRect21.X + c_EL.FoxElement.sImageSize.Width / 2,
                                destRect21.Y + c_EL.FoxElement.sImageSize.Height / 2 - 4,
                                destRect21.X + c_EL.FoxElement.sImageSize.Width / 2,
                                destRect21.Y + c_EL.FoxElement.sImageSize.Height / 2 + 4,
                                Color.FromKnownColor(KnownColor.Yellow), 2.0f);
                        }

                        // Win Condition Rectangle
                        Rectangle destRect22 = Rectangle.Empty;
                        destRect22.X = winPoint.X * tileSize.Width + Tile_World_Panel.AutoScrollPosition.X;
                        destRect22.Y = winPoint.Y * tileSize.Height + Tile_World_Panel.AutoScrollPosition.Y;
                        destRect22.Size = sEndPointSize;


                        if (winPoint != Point.Empty)
                        {
                            mD3D.DrawEmptyRect(destRect22, Color.FromKnownColor(KnownColor.Green), 1.25f);
                        }

                    }
                }
            }
        }

        // Tileset Render  /////////////////////////////////////////////////////////////////////////////
        private void RenderTileSet()
        {   
            mD3D.Sprite.Flush();
            // Drawing Rectangle based off point on tileset.
            Point TileRect = Point.Empty;
            TileRect.X -= Tileset_Panel.AutoScrollPosition.X;
            TileRect.Y -= Tileset_Panel.AutoScrollPosition.Y;

            // Drawing Tileset
            mTM.Draw(nTileSetID, 0, 0, 1.0f, 1.0f, new Rectangle(TileRect, new Size(rTileRect.Width, rTileRect.Height)), 0, 0, 0.0f, 0);

            

            // Drawing white Grid on the Tileset
            for (int x = 0; x < tileSetSize.Width; x++)
            {
                for (int y = 0; y < tileSetSize.Height; y++)
                {
                    // Tile Size converting to Destination Rect
                    Rectangle destRect = Rectangle.Empty;
                    destRect.X = x * tileSize.Width + Tileset_Panel.AutoScrollPosition.X;
                    destRect.Y = y * tileSize.Height + Tileset_Panel.AutoScrollPosition.Y;
                    destRect.Size = tileSize;

                    mD3D.DrawEmptyRect(destRect, Color.FromArgb(255, 60, 60, 60), 0.25f);
                }
            }

            // Selecting tile and changing its Red marker to it
            Rectangle selRect = new Rectangle(new Point(selectedTile.X, selectedTile.Y), new Size(selectedEndTile.X , selectedEndTile.Y ));
            selRect.X = selectedTile.X;
            selRect.Y = selectedTile.Y;
            selRect.X += Tileset_Panel.AutoScrollPosition.X;
            selRect.Y += Tileset_Panel.AutoScrollPosition.Y;

            mD3D.DrawEmptyRect(selRect, Color.Gold, 3.0f);
        }

        // Apply Tile Settings Button ///////////////////////////////////////////////////////////////////
        private void Apply_Tile_Click(object sender, EventArgs e)
        {
            tileSetSize.Width = (int)numericUpDown3.Value;
            tileSetSize.Height = (int)numericUpDown4.Value;

            tileSize.Width = (int)numericUpDown1.Value;
            tileSize.Height = (int)numericUpDown2.Value;

            ScrollingSizeSet();
        }

        // Tileset Click ///////////////////////////////////////////////////////////////////////////////
        private void Tileset_MouseClick(object sender, MouseEventArgs e)
        {
            //Point tempPoint = Point.Empty;
            //tempPoint.X = (e.X - Tileset_Panel.AutoScrollPosition.X) / tileSize.Width;
            //tempPoint.Y = (e.Y - Tileset_Panel.AutoScrollPosition.Y) / tileSize.Height;

            //if (tempPoint.X >= tileSetSize.Width | tempPoint.Y >= tileSetSize.Height)
            //    return;
            //else
            //    selectedTile = tempPoint;
        }

        // Tileset Mouse Up ///////////////////////////////////////////////////////////////////////////////
        private void Tileset_Panel_MouseUp(object sender, MouseEventArgs e)
        {
            bDragging = false;
            //Tileset_Panel.Capture = false;

            while (selectedTile.X % 32 != 0)
            {
                selectedTile.X--;
            }
            
            while (selectedTile.Y % 32 != 0)
            {
                selectedTile.Y--;
            }

            while (selectedEndTile.X % 32 != 0)
            {
                selectedEndTile.X++;
            }

            while (selectedEndTile.Y % 32 != 0)
            {
                selectedEndTile.Y++;
            }

        }

        // Tileset Mouse Down ///////////////////////////////////////////////////////////////////////////////
        private void Tileset_Panel_MouseDown(object sender, MouseEventArgs e)
        {
            //Tileset_Panel.Capture = true;
            
            Point tempPoint = Point.Empty;
            tempPoint.X = (e.X - Tileset_Panel.AutoScrollPosition.X) / tileSize.Width;
            tempPoint.Y = (e.Y - Tileset_Panel.AutoScrollPosition.Y) / tileSize.Height;

            if (tempPoint.X >= tileSetSize.Width | tempPoint.Y >= tileSetSize.Height)
                return;
            else
            {
                selectedTile.X = e.X - Tileset_Panel.AutoScrollPosition.X;
                selectedTile.Y = e.Y - Tileset_Panel.AutoScrollPosition.Y;
            }

            //selectedEndTile.X = selectedTile.X + tileSize.Width;
            //selectedEndTile.Y = selectedTile.Y + tileSize.Height;

            bDragging = true;
        }

        // Tileset Click Drag ////////////////////////////////////////////////////////////////////////////
        private void Tileset_Panel_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left )
            {
                selectedEndTile.X = e.X - Tileset_Panel.AutoScrollPosition.X - selectedTile.X;
                selectedEndTile.Y = e.Y - Tileset_Panel.AutoScrollPosition.Y - selectedTile.Y;
            }
        }

        // World Click /////////////////////////////////////////////////////////////////////////////////
        private void World_MouseClick(object sender, MouseEventArgs e)
        {
            int nTileW = tileSize.Width;
            int nTileH = tileSize.Height;
            int nSelectedW = selectedEndTile.X / nTileW;
            int nSelectedH = selectedEndTile.Y / nTileH;

            Point tempPoint = Point.Empty;

            tempPoint.X = (e.X - Tile_World_Panel.AutoScrollPosition.X) / tileSize.Width;
            tempPoint.Y = (e.Y - Tile_World_Panel.AutoScrollPosition.Y) / tileSize.Height;

            if (tempPoint.X >= mapSize.Width | tempPoint.Y >= mapSize.Height)
                return;
            else
            {
                if (c_EL.HorizontalGateElement.bBrush != false)
                {
                    if (bGateOpen == false)
                        c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.HorizontalGateElement.szEvent + c_EL.HorizontalGateElement.nStringIndiceSize;
                    else
                        c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.HorizontalGateElement.szEvent + c_EL.HorizontalGateElement.nStringIndiceSize + "_Open";
                    c_TM.szTriggerEventID[tempPoint.X, tempPoint.Y] = szCurrentTrigger;
                    c_EL.HorizontalGateElement.nStringIndiceSize = c_EL.HorizontalGateElement.nStringIndiceSize + 1;
                    c_EL.HorizontalGateElement.szTriggerEvent = szCurrentTrigger;
                    bGateOpen = false;
                    checkBox1.Checked = false;
                    //else
                    //{
                    //    //c_EL.HorizontalGateElement.szTriggerEvent = szCurrentTrigger + "_Open";
                    //    bGateOpen = false;
                    //    checkBox1.Checked = false;
                    //}
                    
                    c_EL.HorizontalGateElement.bBrush = false;
                    Horizontal_Gate_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.VerticalGateElement.bBrush != false)
                {
                    if (bGateOpen == false)
                        c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.VerticalGateElement.szEvent + c_EL.VerticalGateElement.nStringIndiceSize;
                    else
                        c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.VerticalGateElement.szEvent + c_EL.VerticalGateElement.nStringIndiceSize + "_Open";
                    c_TM.szTriggerEventID[tempPoint.X, tempPoint.Y] = szCurrentTrigger;
                    c_EL.VerticalGateElement.nStringIndiceSize = c_EL.VerticalGateElement.nStringIndiceSize + 1;
                    bGateOpen = false;
                    checkBox1.Checked = false;
                    
                                        
                    c_EL.VerticalGateElement.bBrush = false;
                    Vertical_Gate_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.PressurePlateElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.PressurePlateElement.szEvent + c_EL.PressurePlateElement.nStringIndiceSize;
                    c_TM.szTriggerEventID[tempPoint.X, tempPoint.Y] = szCurrentTrigger;
                    c_EL.PressurePlateElement.nStringIndiceSize = c_EL.PressurePlateElement.nStringIndiceSize + 1;
                    c_EL.PressurePlateElement.szTriggerEvent = szCurrentTrigger;
                    c_EL.PressurePlateElement.bBrush = false;
                    Pressure_Plate_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.CameraElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CameraElement.szEvent + c_EL.CameraElement.nStringIndiceSize;
                    c_EL.CameraElement.nStringIndiceSize = c_EL.CameraElement.nStringIndiceSize + 1;
                    c_EL.CameraElement.bBrush = false;
                    Security_Cam_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.LeverElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y ] = c_EL.LeverElement.szEvent + c_EL.LeverElement.nStringIndiceSize;
                    c_EL.LeverElement.nStringIndiceSize = c_EL.LeverElement.nStringIndiceSize + 1;
                    c_EL.LeverElement.bBrush = false;
                    Lever_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.SpringElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.SpringElement.szEvent + c_EL.SpringElement.nStringIndiceSize;
                    c_EL.SpringElement.nStringIndiceSize = c_EL.SpringElement.nStringIndiceSize + 1;
                    c_EL.SpringElement.bBrush = false;
                    Spring_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.BalloonElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.BalloonElement.szEvent + c_EL.BalloonElement.nStringIndiceSize;
                    c_EL.BalloonElement.nStringIndiceSize = c_EL.BalloonElement.nStringIndiceSize + 1;
                    c_EL.BalloonElement.bBrush = false;
                    Balloon_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.CrateElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CrateElement.szEvent + c_EL.CrateElement.nStringIndiceSize;
                    c_EL.CrateElement.nStringIndiceSize = c_EL.CrateElement.nStringIndiceSize + 1;
                    c_EL.CrateElement.bBrush = false;
                    Crate_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.PogoElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.PogoElement.szEvent + c_EL.PogoElement.nStringIndiceSize;
                    c_EL.PogoElement.nStringIndiceSize = c_EL.PogoElement.nStringIndiceSize + 1;
                    c_EL.PogoElement.bBrush = false;
                    PU_Pogo_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.CheetahElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CheetahElement.szEvent + c_EL.CheetahElement.nStringIndiceSize;
                    c_EL.CheetahElement.nStringIndiceSize = c_EL.CheetahElement.nStringIndiceSize + 1;
                    c_EL.CheetahElement.bBrush = false;
                    PU_Cheetah_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.FizzyElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.FizzyElement.szEvent + c_EL.FizzyElement.nStringIndiceSize;
                    c_EL.FizzyElement.nStringIndiceSize = c_EL.FizzyElement.nStringIndiceSize + 1;
                    c_EL.FizzyElement.bBrush = false;
                    Fizzy_Drink_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.GreasedPigElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.GreasedPigElement.szEvent + c_EL.GreasedPigElement.nStringIndiceSize;
                    c_EL.GreasedPigElement.nStringIndiceSize = c_EL.GreasedPigElement.nStringIndiceSize + 1;
                    c_EL.GreasedPigElement.bBrush = false;
                    Greased_Pig_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.FlubbElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.FlubbElement.szEvent + c_EL.FlubbElement.nStringIndiceSize;
                    c_EL.FlubbElement.nStringIndiceSize = c_EL.FlubbElement.nStringIndiceSize + 1;
                    c_EL.FlubbElement.bBrush = false;
                    Flubber_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.WaterGunElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.WaterGunElement.szEvent + c_EL.WaterGunElement.nStringIndiceSize;
                    c_EL.WaterGunElement.nStringIndiceSize = c_EL.WaterGunElement.nStringIndiceSize + 1;
                    c_EL.WaterGunElement.bBrush = false;
                    button21.BackColor = ColorElementButton;
                    
                }
                else if (c_EL.CottonCandyElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CottonCandyElement.szEvent + c_EL.CottonCandyElement.nStringIndiceSize;
                    c_EL.CottonCandyElement.nStringIndiceSize = c_EL.CottonCandyElement.nStringIndiceSize + 1;
                    c_EL.CottonCandyElement.bBrush = false;
                    button20.BackColor = ColorElementButton;
                }
                else if (c_EL.BalloonGunElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.BalloonGunElement.szEvent + c_EL.BalloonGunElement.nStringIndiceSize;
                    c_EL.BalloonGunElement.nStringIndiceSize = c_EL.BalloonGunElement.nStringIndiceSize + 1;
                    c_EL.BalloonGunElement.bBrush = false;
                    button19.BackColor = ColorElementButton;
                }
                else if (c_EL.RatElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.RatElement.szEvent + c_EL.RatElement.nStringIndiceSize;
                    c_EL.RatElement.nStringIndiceSize = c_EL.RatElement.nStringIndiceSize + 1;
                    c_EL.RatElement.bBrush = false;
                    Rat_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.ClownElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.ClownElement.szEvent + c_EL.ClownElement.nStringIndiceSize;
                    c_EL.ClownElement.nStringIndiceSize = c_EL.ClownElement.nStringIndiceSize + 1;
                    c_EL.ClownElement.bBrush = false;
                    Clown_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.HoodlumElement.bBrush != false)
                {
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.HoodlumElement.szEvent + c_EL.HoodlumElement.nStringIndiceSize;
                    c_EL.HoodlumElement.nStringIndiceSize = c_EL.HoodlumElement.nStringIndiceSize + 1;
                    c_EL.HoodlumElement.bBrush = false;
                    Hoodlum_Button.BackColor = ColorElementButton;
                }
                else if (c_EL.CheckpointElement.bBrush != false)
                {
                    
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CheckpointElement.szEvent + c_EL.CheckpointElement.nStringIndiceSize;
                    c_EL.CheckpointElement.nStringIndiceSize = c_EL.CheckpointElement.nStringIndiceSize + 1;
                    c_EL.CheckpointElement.bBrush = false;
                    button3.BackColor = ColorElementButton;
                }

                // Corinne Brush
                else if (c_EL.CorinneElement.bBrush != false && !bIsCorinneIn )
                {
                    // Check to see if there's any Corinnes on the Map and delete if so, then reassign.
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.CorinneElement.szEvent;
                    c_EL.CorinneElement.bBrush = false;
                    button12.BackColor = ColorElementButton;
                }
                // Fox Brush
                else if (c_EL.FoxElement.bBrush != false && !bIsFoxIn)
                {
                    // Check to see if there's any Foxes on the Map and delete if so, then reassign.
                    c_TM.szEventID[tempPoint.X, tempPoint.Y] = c_EL.FoxElement.szEvent;
                    c_EL.FoxElement.bBrush = false;
                    button13.BackColor = ColorElementButton;
                }

                // Tile Eraser Brush Logic
                else if (bEraseTileBrush != false)
                {
                    if (c_TM.nPlayTileImageID[tempPoint.X, tempPoint.Y] != -1)
                    {
                        c_TM.nPlayTileImageID[tempPoint.X, tempPoint.Y] = -1;
                        c_TM.rPlayRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;
                    }

                    if (c_TM.nMidBackTileImageID[tempPoint.X, tempPoint.Y] != -1)
                    {
                        c_TM.nMidBackTileImageID[tempPoint.X, tempPoint.Y] = -1;
                        c_TM.rMidBackRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;
                    }

                    if (c_TM.nFrontTileImageID[tempPoint.X, tempPoint.Y] != -1)
                    {
                        c_TM.nFrontTileImageID[tempPoint.X, tempPoint.Y] = -1;
                        c_TM.rFrontRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;
                    }
                }

                // Collision Brush Logic
                else if (bCollisionBrush != false)
                {
                    if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.NONCOLLIDE)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.NONCOLLIDE;
                    }
                        

                    else if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.COLLIDE)
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.COLLIDE;

                }

                // Gravity Flip Brush Logic
                else if (bGravityFlipBrush != false)
                {
                    if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.GRAVITYON)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.GRAVITYON;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        button1.BackColor = Color.FromName("Transparent");
                        bGravityFlipBrush = false;
                    }

                    else if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] == (int)CollideType.GRAVITYON)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.NONCOLLIDE;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        bGravityFlipBrush = false;
                    }
                }

                // Jump Node Brush Logic
                else if (bJumpNodeBrush != false)
                {
                    if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.JUMP)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.JUMP;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        Jump_Node.BackColor = Color.FromName("Transparent");
                        bJumpNodeBrush = false;
                    }

                    else if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] == (int)CollideType.JUMP)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.NONCOLLIDE;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        bJumpNodeBrush = false;
                    }
                }

                // Stop Node Brush Logic
                else if (bStopNodeBrush != false)
                {
                    if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.STOP)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.STOP;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        Stop_Node.BackColor = Color.FromName("Transparent");
                        bStopNodeBrush = false;
                    }

                    else if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] == (int)CollideType.STOP)
                    {
                        c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.NONCOLLIDE;
                        c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                        bStopNodeBrush = false;
                    }
                }

                // Erase Element Brush Logic
                else if (bEraseElementBrush != false)
                {
                    if( c_TM.szEventID[tempPoint.X, tempPoint.Y] != null )
                        c_TM.szEventID[tempPoint.X, tempPoint.Y] = null;
                    bEraseElementBrush = false;
                    Erase_Element.BackColor = Color.FromName("Transparent");
                }

                // Win Node Brush Logic
                else if (bEndPointBrush != false)
                {
                    if (tempPoint.Y == 0)
                        winPoint = new Point(tempPoint.X, tempPoint.Y);
                    bEndPointBrush = false;
                    Win_Node.BackColor = Color.FromName("Transparent");
                }

                // Play Brush Logic
                else if (bPlayBrush != false)
                {
                    if (tempPoint.X + nSelectedW < mapSize.Width + 1 && tempPoint.Y + nSelectedH < mapSize.Height + 1)
                    {
                        for (int i = 0; i < nSelectedW; i++)
                        {
                            for (int j = 0; j < nSelectedH; j++)
                            {
                                c_TM.rPlayRect[tempPoint.X + i, tempPoint.Y + j] =
                                    new Rectangle(new Point(selectedTile.X + i * tileSize.Width, selectedTile.Y + j * tileSize.Height),
                                        new Size(tileSize.Width, tileSize.Height));
                                c_TM.nPlayTileImageID[tempPoint.X + i, tempPoint.Y + j] = nTileSetID;
                                c_TM.nLayerType[tempPoint.X + i, tempPoint.Y + j] = (int)LayerType.PLAY_LAYER;
                                c_TM.map[tempPoint.X + i, tempPoint.Y + j] = selectedTile;
                            }
                        }
                    }
                }

                // Front Brush Logic
                else if (bFrontBrush != false)
                {
                    if (tempPoint.X + nSelectedW < mapSize.Width + 1 && tempPoint.Y + nSelectedH < mapSize.Height + 1)
                    {
                        for (int i = 0; i < nSelectedW; i++)
                        {
                            for (int j = 0; j < nSelectedH; j++)
                            {
                                c_TM.rFrontRect[tempPoint.X + i, tempPoint.Y + j] =
                                    new Rectangle(new Point(selectedTile.X + i * tileSize.Width, selectedTile.Y + j * tileSize.Height),
                                        new Size(tileSize.Width, tileSize.Height));
                                c_TM.nFrontTileImageID[tempPoint.X + i, tempPoint.Y + j] = nTileSetID;
                                c_TM.nLayerType[tempPoint.X + i, tempPoint.Y + j] = (int)LayerType.FRONT_LAYER;
                                c_TM.map[tempPoint.X + i, tempPoint.Y + j] = selectedTile;
                            }
                        }
                    }
                }

                // Mid Layer Brush Logic
                else if (bMidBrush != false)
                {
                    if (tempPoint.X + nSelectedW < mapSize.Width + 1 && tempPoint.Y + nSelectedH < mapSize.Height + 1)
                    {
                        for (int i = 0; i < nSelectedW; i++)
                        {
                            for (int j = 0; j < nSelectedH; j++)
                            {
                                c_TM.rMidBackRect[tempPoint.X + i, tempPoint.Y + j] =
                                    new Rectangle(new Point(selectedTile.X + i * tileSize.Width, selectedTile.Y + j * tileSize.Height),
                                        new Size(tileSize.Width, tileSize.Height));
                                c_TM.nMidBackTileImageID[tempPoint.X + i, tempPoint.Y + j] = nTileSetID;
                                c_TM.nLayerType[tempPoint.X + i, tempPoint.Y + j] = (int)LayerType.MID_LAYER;
                                c_TM.map[tempPoint.X + i, tempPoint.Y + j] = selectedTile;
                            }
                        }
                    }
                }
            }   

        }

        // World Click Drag ////////////////////////////////////////////////////////////////////////////
        private void World_MouseMove(object sender, MouseEventArgs e)
        {
            int nTileW = tileSize.Width;
            int nTileH = tileSize.Height;
            int nSelectedW = selectedEndTile.X / nTileW;
            int nSelectedH = selectedEndTile.Y / nTileH;

            // Checking to see if Hover Brush Draw Rect fits in Tile World
            Point tempPoint2 = Point.Empty;
            tempPoint2.X = (e.X ) / nTileW;
            tempPoint2.Y = (e.Y ) / nTileH;

            if (tempPoint2.X + nSelectedW < mapSize.Width + 1 && tempPoint2.Y + nSelectedH < mapSize.Height + 1)
                bIsTileOutsideMap = false;
            else
                bIsTileOutsideMap = true;

            // Culling Hover Brush above Scrollbar
            if ( e.Y >= Tile_World_Panel.Height )
            {
                selectedWorldDrag.X = -2000;
                selectedWorldDrag.Y = -2000;
            }
            else 
            {
                int tempcheckX = e.X - Tile_World_Panel.AutoScrollPosition.X / nTileW;
                int tempcheckY = e.Y - Tile_World_Panel.AutoScrollPosition.Y / nTileH;
                while (tempcheckX % 32 != 0 )
                {
                    tempcheckX--;
                }
                while (tempcheckY % 32 != 0)
                {
                    tempcheckY--;
                }

                selectedWorldDrag.X = tempcheckX;
                selectedWorldDrag.Y = tempcheckY;
            }

            if (e.Button == MouseButtons.Left)
            {
                try
                {
                    Point tempPoint = Point.Empty;

                    tempPoint.X = (e.X - Tile_World_Panel.AutoScrollPosition.X) / tileSize.Width;
                    tempPoint.Y = (e.Y - Tile_World_Panel.AutoScrollPosition.Y) / tileSize.Height;

                    if (tempPoint.X >= mapSize.Width | tempPoint.Y >= mapSize.Height)
                        return;
                    else
                    {
                        if (bPlayBrush != false && bCollisionBrush != true && bEraseTileBrush != true)
                        {
                            // Dont draw anything if any Game Element buttons are pressed.
                            if (c_EL.HorizontalGateElement.bBrush == false && c_EL.PressurePlateElement.bBrush == false && c_EL.CameraElement.bBrush == false &&
                                c_EL.LeverElement.bBrush == false && c_EL.SpringElement.bBrush == false && c_EL.BalloonElement.bBrush == false &&
                                c_EL.PogoElement.bBrush == false && c_EL.CheetahElement.bBrush == false && c_EL.FizzyElement.bBrush == false &&
                                c_EL.GreasedPigElement.bBrush == false && c_EL.FlubbElement.bBrush == false && c_EL.WaterGunElement.bBrush == false &&
                                c_EL.CottonCandyElement.bBrush == false && c_EL.BalloonGunElement.bBrush == false && c_EL.RatElement.bBrush == false &&
                                c_EL.ClownElement.bBrush == false && c_EL.HoodlumElement.bBrush == false && c_EL.CheckpointElement.bBrush == false && 
                                c_EL.CorinneElement.bBrush == false && bCollisionBrush == false )
                            {
                                c_TM.map[tempPoint.X, tempPoint.Y] = selectedTile;
                                c_TM.nPlayTileImageID[tempPoint.X, tempPoint.Y] = nTileSetID;
                                c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.PLAY_LAYER;
                               
                                c_TM.rPlayRect[tempPoint.X, tempPoint.Y] = new Rectangle(new Point(selectedTile.X, selectedTile.Y), 
                                                                                        new Size(selectedEndTile.X , selectedEndTile.Y ));
                                
                            }
                        }

                        else if (bCollisionBrush != false && bEraseTileBrush != true )
                        {
                            if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.COLLIDE)
                                c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.COLLIDE;
                            //else if (c_TM.nCollideType[tempPoint.X, tempPoint.Y] != (int)CollideType.NONCOLLIDE)
                            //    c_TM.nCollideType[tempPoint.X, tempPoint.Y] = (int)CollideType.NONCOLLIDE;

                        }

                        else if (bFrontBrush != false && bCollisionBrush != true && bEraseTileBrush != true)
                        {
                            c_TM.map[tempPoint.X, tempPoint.Y] = selectedTile;
                            c_TM.nFrontTileImageID[tempPoint.X, tempPoint.Y] = nTileSetID;
                            c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.FRONT_LAYER;
                            c_TM.rFrontRect[tempPoint.X, tempPoint.Y] = new Rectangle(new Point(selectedTile.X, selectedTile.Y),
                                                                                        new Size(selectedEndTile.X, selectedEndTile.Y));
                        }

                        else if (bMidBrush != false && bCollisionBrush != true && bEraseTileBrush != true)
                        {
                            c_TM.map[tempPoint.X, tempPoint.Y] = selectedTile;
                            c_TM.nMidBackTileImageID[tempPoint.X, tempPoint.Y] = nTileSetID;
                            c_TM.nLayerType[tempPoint.X, tempPoint.Y] = (int)LayerType.MID_LAYER;
                            c_TM.rMidBackRect[tempPoint.X, tempPoint.Y] = new Rectangle(new Point(selectedTile.X, selectedTile.Y),
                                                                                        new Size(selectedEndTile.X, selectedEndTile.Y));
                        }

                        else if (bEraseTileBrush != false && bCollisionBrush != true)
                        {
                            if (c_TM.nPlayTileImageID[tempPoint.X, tempPoint.Y] != -1)
                            {
                                c_TM.nPlayTileImageID[tempPoint.X, tempPoint.Y] = -1;
                                c_TM.rPlayRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;

                            }

                            if (c_TM.nMidBackTileImageID[tempPoint.X, tempPoint.Y] != -1)
                            {
                                c_TM.nMidBackTileImageID[tempPoint.X, tempPoint.Y] = -1;
                                c_TM.rMidBackRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;

                            }

                            if (c_TM.nFrontTileImageID[tempPoint.X, tempPoint.Y] != -1)
                            {
                                c_TM.nFrontTileImageID[tempPoint.X, tempPoint.Y] = -1;
                                c_TM.rFrontRect[tempPoint.X, tempPoint.Y] = Rectangle.Empty;
                            }

                        }
                    }

                }
                catch (IndexOutOfRangeException)
                {

                }

                
            }
        }

        private void Tile_World_Panel_MouseLeave(object sender, EventArgs e)
        {

            selectedWorldDrag = new Point(-2000, -2000);
        }

        private void Tile_World_Panel_MouseDown(object sender, MouseEventArgs e)
        {

        }

        private void Tile_World_Panel_MouseUp(object sender, MouseEventArgs e)
        {
            //if (c_TM.nCollideType[selectedWorldDrag.X / 32, selectedWorldDrag.Y / 32] == (int)CollideType.COLLIDE)
            //    c_TM.nCollideType[selectedWorldDrag.X / 32, selectedWorldDrag.Y / 32] = (int)CollideType.NONCOLLIDE;
            //else
            //    c_TM.nCollideType[selectedWorldDrag.X / 32, selectedWorldDrag.Y / 32] = (int)CollideType.COLLIDE;
        }

        // Scrolling Bars Size Readjustment ////////////////////////////////////////////////////////////
        void ScrollingSizeSet()
        {
            Size TempTileSize = new Size(tileSetSize.Width * tileSize.Width, tileSetSize.Height * tileSize.Height);
            Size TempSize = new Size(mapSize.Width * tileSize.Width, mapSize.Height * tileSize.Height);

            Tileset_Panel.AutoScrollMinSize = TempTileSize;
            Tile_World_Panel.AutoScrollMinSize = TempSize;
        }

        // New File Click //////////////////////////////////////////////////////////////////////////////
        private void newWorldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // Reset Tile Map to 800x600 in 32x32 Tiles.
            mapSize.Width = 25;
            mapSize.Height = 19;

            for (int x = 0; x < mapSize.Width; x++)
            {
                for (int y = 0; y < mapSize.Height; y++)
                {
                    c_TM.nPlayTileImageID[x, y] = -1;
                    c_TM.nCollideType[x, y] = (int)CollideType.NONCOLLIDE;
                    c_TM.nLayerType[x, y] = -1;
                    c_TM.szEventID[x, y] = string.Empty;
                    c_TM.szTriggerEventID[x, y] = string.Empty;
                }
            }

            winPoint = new Point(0, 0);

            ScrollingSizeSet();
            bBeginLayer = true;
            bFrontLayer = false;
            Front_Layer.Checked = false;
            bPlayLayer = false;
            Play_Layer.Checked = false;
            bMidLayer = false;
            Mid_Back_Layer.Checked = false;
            bBackLayer = false;
            Background_Layer.Checked = false;
            bCollisionBrush = false;
        }

        // Exit Editor Click ///////////////////////////////////////////////////////////////////////////
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        // Add Tile Map Page Button ////////////////////////////////////////////////////////////////////
        private void Add_Tile_Page_Click(object sender, EventArgs e)
        {
            Size tempMapSize = Size.Empty;
            tempMapSize = mapSize;
            mapSize = new Size(tempMapSize.Width + 25, tempMapSize.Height);

            // Temporary values to hold what the Tile Map is about to equate
            Point[,] tempMap = new Point[mapSize.Width, mapSize.Height];
            Rectangle[,] tempPlayRectSize = new Rectangle[mapSize.Width, mapSize.Height];
            Rectangle[,] tempFrontRectSize = new Rectangle[mapSize.Width, mapSize.Height];
            Rectangle[,] tempMidBackRectSize = new Rectangle[mapSize.Width, mapSize.Height];
            int[,] tempImgIDsize = new int[mapSize.Width, mapSize.Height];
            int[,] tempFrontImgIDsize = new int[mapSize.Width, mapSize.Height];
            int[,] tempMidBackIDsize = new int[mapSize.Width, mapSize.Height];

            int[,] tempTileCollideType = new int[mapSize.Width, mapSize.Height];

            int[,] tempLayerType = new int[mapSize.Width, mapSize.Height];

            string[,] tempEventID = new string[mapSize.Width, mapSize.Height];
            string[,] tempTriggerEventID = new string[mapSize.Width, mapSize.Height];

            // Initializing a new Tile page
            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    if (x >= mapSize.Width - 25)
                    {
                        // Initializing temporary values' new indice sizes' special constructors
                        tempImgIDsize[x, y] = -1;
                        tempFrontImgIDsize[x, y] = -1;
                        tempMidBackIDsize[x, y] = -1;

                        tempTileCollideType[x, y] = (int)CollideType.NONCOLLIDE;

                    }
                }
            }

            for (int x = 0; x < tempMapSize.Width; ++x)
            {
                for (int y = 0; y < tempMapSize.Height; ++y)
                {
                    if (x >= mapSize.Width || y >= mapSize.Height)
                        continue;

                    // Setting temporary values new indice sizes to the Tile Map
                    tempMap[x, y] = c_TM.map[x, y];
                    tempPlayRectSize[x, y] = c_TM.rPlayRect[x, y];
                    tempFrontRectSize[x, y] = c_TM.rFrontRect[x, y];
                    tempMidBackRectSize[x, y] = c_TM.rMidBackRect[x, y];

                    tempImgIDsize[x, y] = c_TM.nPlayTileImageID[x, y];
                    tempFrontImgIDsize[x, y] = c_TM.nFrontTileImageID[x, y];
                    tempMidBackIDsize[x, y] = c_TM.nMidBackTileImageID[x, y];

                    tempLayerType[x, y] = c_TM.nLayerType[x, y];
                    tempTileCollideType[x, y] = c_TM.nCollideType[x, y];
                    tempEventID[x, y] = c_TM.szEventID[x, y];
                    tempTriggerEventID[x, y] = c_TM.szTriggerEventID[x, y];
                }
            }

            // Setting new Tile Map's data members to the temporary's values
            c_TM.map = tempMap;
            c_TM.rPlayRect = tempPlayRectSize;
            c_TM.rFrontRect = tempFrontRectSize;
            c_TM.rMidBackRect = tempMidBackRectSize;

            c_TM.nPlayTileImageID = tempImgIDsize;
            c_TM.nMidBackTileImageID = tempMidBackIDsize;
            c_TM.nFrontTileImageID = tempFrontImgIDsize;

            c_TM.nCollideType = tempTileCollideType;
            c_TM.nLayerType = tempLayerType;
            c_TM.szEventID = tempEventID;
            c_TM.szTriggerEventID = tempTriggerEventID;

            ScrollingSizeSet();

            c_EL.BackgroundIMG.rIRect = new Rectangle(new Point(0, 0),
                new Size(c_EL.BackgroundIMG.rIRect.Width + c_EL.BackgroundIMG.sImageSize.Width + 75,
                            c_EL.BackgroundIMG.sImageSize.Height));

            Tile_World_Panel.Focus();
        }

        // Front Layer Checkbox ////////////////////////////////////////////////////////////////////////
        private void Front_Layer_CheckedChanged(object sender, EventArgs e)
        {
            if (Front_Layer.Checked != false)
            {
                bFrontLayer = true;
                bBeginLayer = false;

                bFrontBrush = true;
                Front_Brush.Checked = true;
                Front_Brush.Enabled = true;
            }
            else
            {
                bFrontLayer = false;
                bFrontBrush = false;
                Front_Brush.Checked = false;
                Front_Brush.Enabled = false;
            }
        }

        // Play Layer Checkbox /////////////////////////////////////////////////////////////////////////
        private void Play_Layer_CheckedChanged(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                bBeginLayer = false;
                bPlayLayer = true;
                label21.ForeColor = Color.FromName("Black");
                Collide_Brush.BackColor = Color.FromName("Transparent");
                Erase_Element.BackColor = ColorElementButton;
                Horizontal_Gate_Button.BackColor = ColorElementButton;
                Vertical_Gate_Button.BackColor = ColorElementButton;
                Pressure_Plate_Button.BackColor = ColorElementButton;
                Security_Cam_Button.BackColor = ColorElementButton;
                Lever_Button.BackColor = ColorElementButton;
                Spring_Button.BackColor = ColorElementButton;
                Crate_Button.BackColor = ColorElementButton;
                Balloon_Button.BackColor = ColorElementButton;
                PU_Pogo_Button.BackColor = ColorElementButton;
                PU_Cheetah_Button.BackColor = ColorElementButton;
                Fizzy_Drink_Button.BackColor = ColorElementButton;
                Greased_Pig_Button.BackColor = ColorElementButton;
                Flubber_Button.BackColor = ColorElementButton;
                Rat_Button.BackColor = ColorElementButton;
                Clown_Button.BackColor = ColorElementButton;
                Hoodlum_Button.BackColor = ColorElementButton;
                Jump_Node.BackColor = Color.FromName("Transparent");
                Glass_Node.BackColor = Color.FromName("Transparent");
                Stop_Node.BackColor = Color.FromName("Transparent");
                button1.BackColor = Color.FromName("Transparent");
                button21.BackColor = ColorElementButton;
                button20.BackColor = ColorElementButton;
                button19.BackColor = ColorElementButton;
                button3.BackColor = ColorElementButton;
                Win_Node.BackColor = ColorElementButton;
                button12.BackColor = ColorElementButton;
                button13.BackColor = ColorElementButton;
                groupBox1.ForeColor = Color.FromName("Black");
                groupBox2.ForeColor = Color.FromName("Black");
                groupBox4.ForeColor = Color.FromName("Black");
                groupBox5.ForeColor = Color.FromName("Black");
                Nodes.ForeColor = Color.FromName("Black");
                Game_Elements.ForeColor = Color.FromName("Black");
                Tile_Eraser.BackColor = Color.FromName("Transparent");

                Collide_Brush.BackgroundImageLayout = ImageLayout.Stretch;
                Play_Layer.ForeColor = Color.FromName("Firebrick");

                label1.ForeColor = Color.FromName("Black");
                label2.ForeColor = Color.FromName("Black");
                label3.ForeColor = Color.FromName("Black");
                label4.ForeColor = Color.FromName("Black");
                label5.ForeColor = Color.FromName("Black");
                label6.ForeColor = Color.FromName("Black");
                label7.ForeColor = Color.FromName("Black");
                label8.ForeColor = Color.FromName("Black");
                label9.ForeColor = Color.FromName("Black");
                label10.ForeColor = Color.FromName("Black");
                label11.ForeColor = Color.FromName("Black");
                label12.ForeColor = Color.FromName("Black");
                label13.ForeColor = Color.FromName("Black");
                label14.ForeColor = Color.FromName("Black");
                label15.ForeColor = Color.FromName("Black");
                label16.ForeColor = Color.FromName("Black");
                label17.ForeColor = Color.FromName("Black");
                label18.ForeColor = Color.FromName("Black");
                label19.ForeColor = Color.FromName("Black");
                label20.ForeColor = Color.FromName("Black");
                label31.ForeColor = Color.FromName("Black");
                label32.ForeColor = Color.FromName("Black");
                label33.ForeColor = Color.FromName("Black");
                label35.ForeColor = Color.FromName("Black");
                label26.ForeColor = Color.FromName("Black");
                label36.ForeColor = Color.FromName("Black");
                label37.ForeColor = Color.FromName("Black");
                label38.ForeColor = Color.FromName("Black");

                bPlayBrush = true;
                Play_Brush.Checked = true;
                Play_Brush.Enabled = true;
            }
            else
            {
                bPlayLayer = false;
                bPlayBrush = false;
                Play_Brush.Checked = false;
                Play_Brush.Enabled = false;

                label21.ForeColor = Color.FromName("DarkGray");
                Collide_Brush.BackColor = Color.FromName("DarkGray");
                Erase_Element.BackColor = Color.FromName("DarkGray");
                Collide_Brush.BackgroundImageLayout = ImageLayout.None;
                Play_Layer.ForeColor = Color.FromName("DarkGray");
                Horizontal_Gate_Button.BackColor = Color.FromName("DarkGray");
                Vertical_Gate_Button.BackColor = Color.FromName("DarkGray");
                Pressure_Plate_Button.BackColor = Color.FromName("DarkGray");
                Security_Cam_Button.BackColor = Color.FromName("DarkGray");
                Lever_Button.BackColor = Color.FromName("DarkGray");
                Spring_Button.BackColor = Color.FromName("DarkGray");
                Crate_Button.BackColor = Color.FromName("DarkGray");
                Balloon_Button.BackColor = Color.FromName("DarkGray");
                PU_Pogo_Button.BackColor = Color.FromName("DarkGray");
                PU_Cheetah_Button.BackColor = Color.FromName("DarkGray");
                Fizzy_Drink_Button.BackColor = Color.FromName("DarkGray");
                Greased_Pig_Button.BackColor = Color.FromName("DarkGray");
                Flubber_Button.BackColor = Color.FromName("DarkGray");
                Rat_Button.BackColor = Color.FromName("DarkGray");
                Clown_Button.BackColor = Color.FromName("DarkGray");
                Hoodlum_Button.BackColor = Color.FromName("DarkGray");
                Jump_Node.BackColor = Color.FromName("DarkGray");
                Glass_Node.BackColor = Color.FromName("DarkGray");
                Stop_Node.BackColor = Color.FromName("DarkGray");
                button1.BackColor = Color.FromName("DarkGray");
                button21.BackColor = Color.FromName("DarkGray");
                button20.BackColor = Color.FromName("DarkGray");
                button19.BackColor = Color.FromName("DarkGray");
                button3.BackColor = Color.FromName("DarkGray");
                Win_Node.BackColor = Color.FromName("DarkGray");
                button12.BackColor = Color.FromName("DarkGray");
                button13.BackColor = Color.FromName("DarkGray");

                bCollisionBrush = false;

                label1.ForeColor = Color.FromName("Silver");
                label2.ForeColor = Color.FromName("Silver");
                label3.ForeColor = Color.FromName("Silver");
                label4.ForeColor = Color.FromName("Silver");
                label5.ForeColor = Color.FromName("Silver");
                label6.ForeColor = Color.FromName("Silver");
                label7.ForeColor = Color.FromName("Silver");
                label8.ForeColor = Color.FromName("Silver");
                label9.ForeColor = Color.FromName("Silver");
                label10.ForeColor = Color.FromName("Silver");
                label11.ForeColor = Color.FromName("Silver");
                label12.ForeColor = Color.FromName("Silver");
                label13.ForeColor = Color.FromName("Silver");
                label14.ForeColor = Color.FromName("Silver");
                label15.ForeColor = Color.FromName("Silver");
                label16.ForeColor = Color.FromName("Silver");
                label17.ForeColor = Color.FromName("Silver");
                label18.ForeColor = Color.FromName("Silver");
                label19.ForeColor = Color.FromName("Silver");
                label20.ForeColor = Color.FromName("Silver");
                label31.ForeColor = Color.FromName("Silver");
                label32.ForeColor = Color.FromName("Silver");
                label33.ForeColor = Color.FromName("Silver");
                label35.ForeColor = Color.FromName("Silver");
                label26.ForeColor = Color.FromName("Silver");
                Tile_Eraser.BackColor = Color.FromName("Silver");

                label36.ForeColor = Color.FromName("Silver");
                label37.ForeColor = Color.FromName("Silver");
                label38.ForeColor = Color.FromName("Silver");

                groupBox1.ForeColor = Color.FromName("Silver");
                groupBox2.ForeColor = Color.FromName("Silver");
                groupBox4.ForeColor = Color.FromName("Silver");
                groupBox5.ForeColor = Color.FromName("Silver");
                Nodes.ForeColor = Color.FromName("Silver");
                Game_Elements.ForeColor = Color.FromName("Silver");
            }
        }

        // Mid Layer Checkbox ///////////////////////////////////////////////////////////////////////////
        private void Mid_Back_Layer_CheckedChanged(object sender, EventArgs e)
        {
            if (Mid_Back_Layer.Checked != false)
            {
                bMidLayer = true;
                bBeginLayer = false;

                bMidBrush = true;
                Mid_Back_Brush.Checked = true;
                Mid_Back_Brush.Enabled = true;
            }
            else
            {
                bMidLayer = false;
                bMidBrush = false;
                Mid_Back_Brush.Checked = false;
                Mid_Back_Brush.Enabled = false;
            }
        }

        // Background Layer Checkbox ////////////////////////////////////////////////////////////////////
        private void Background_Layer_CheckedChanged(object sender, EventArgs e)
        {
            if (Background_Layer.Checked != false)
            {
                if (c_EL.BackgroundIMG.nImageID != -1)
                    bBackLayer = true;
                else
                {
                    OpenFileDialog dlg = new OpenFileDialog();

                    if (DialogResult.OK == dlg.ShowDialog())
                    {
                        c_EL.BackgroundIMG.szFilename =  "Background.png";
                        c_EL.BackgroundIMG.nImageID = mTM.LoadTexture(dlg.FileName, 0);
                        c_EL.BackgroundIMG.sImageSize = new Size(mTM.GetTextureWidth(c_EL.BackgroundIMG.nImageID), mTM.GetTextureHeight(c_EL.BackgroundIMG.nImageID));
                        c_EL.BackgroundIMG.rIRect = new Rectangle(new Point(0, 0), new Size(800, 600));
                        if (c_EL.BackgroundIMG.nImageID != -1)
                        {
                            bBeginLayer = false;
                            bBackLayer = true;
                        }
                    }
                }
                Background_Layer.ForeColor = Color.FromName("SteelBlue");
                Load_Background.BackColor = Color.FromName("DarkGray");
            }
            else
            {
                bBackLayer = false;
                Background_Layer.ForeColor = Color.FromName("Silver");
                Load_Background.BackColor = Color.FromName("LightGray");

            }
        }

        // Background Load Button ///////////////////////////////////////////////////////////////////////
        private void Load_Background_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                //c_EL.BackgroundIMG.szFilename = dlg.FileName;
                c_EL.BackgroundIMG.nImageID = mTM.LoadTexture(dlg.FileName, 0);
                c_EL.BackgroundIMG.sImageSize = new Size(mTM.GetTextureWidth(c_EL.BackgroundIMG.nImageID), mTM.GetTextureHeight(c_EL.BackgroundIMG.nImageID));
                c_EL.BackgroundIMG.rIRect = new Rectangle(new Point(0, 0), new Size(800, 600));
            }
        }

        // Collision Brush Button ///////////////////////////////////////////////////////////////////////
        private void Collide_Brush_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                Collide_Brush.BackColor = Color.FromName("Red");
                bCollisionBrush = !bCollisionBrush;
            }
            if (bCollisionBrush != true)
            {
                Collide_Brush.BackColor = Color.FromName("Transparent");

            }
        }

        // Erase Element Brush Button ///////////////////////////////////////////////////////////////////
        private void Erase_Element_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                // Reset Collision Brush
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                bEraseElementBrush = !bEraseElementBrush;
                Erase_Element.BackColor = Color.FromName("Red");
            }

            if (bEraseElementBrush != true)
            {
                Erase_Element.BackColor = Color.FromName("Transparent");
            }
        }

        // Win Condition Node
        private void Win_Node_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                // Reset Collision Brush
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                bEndPointBrush = !bEndPointBrush;
                Win_Node.BackColor = Color.FromName("Red");
                rEndPointRect.X = 0;
                rEndPointRect.Y = 0;
                rEndPointRect.Width = 32;
                rEndPointRect.Height = 608;
                sEndPointSize = new Size(32, 608);
            }

            if (bEndPointBrush != true)
            {
                Win_Node.BackColor = Color.FromName("Transparent");
            }
        }

        // Horizontal Gate Button
        private void button7_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Horizontal_Gate_Button.BackColor = Color.FromName("Red");
                c_EL.HorizontalGateElement.bBrush = true;
                
            }
        }

        // Vertical Gate Button
        private void button2_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Vertical_Gate_Button.BackColor = Color.FromName("Red");
                c_EL.VerticalGateElement.bBrush = true;
                
            }
        }

        // Pressure Plate Button
        private void button8_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Pressure_Plate_Button.BackColor = Color.FromName("Red");
                c_EL.PressurePlateElement.bBrush = true;
            }
        }

        // Security Camera Button
        private void button10_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;


                Security_Cam_Button.BackColor = Color.FromName("Red");
                c_EL.CameraElement.bBrush = true;
            }
        }

        // Lever Button
        private void button9_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Lever_Button.BackColor = Color.FromName("Red");
                szCurrentTrigger = c_EL.LeverElement.szEvent + c_EL.LeverElement.nStringIndiceSize;
                c_EL.LeverElement.bBrush = true;
            }
        }

        // Spring Button
        private void button11_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                {
                    bCollisionBrush = false;
                }

                Spring_Button.BackColor = Color.FromName("Red");
                c_EL.SpringElement.bBrush = true;
            }
        }

        // Crate Button
        private void Crate_Button_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                {
                    bCollisionBrush = false;
                }

                Crate_Button.BackColor = Color.FromName("Red");
                c_EL.CrateElement.bBrush = true;
            }
        }

        // Balloon Button
        private void Balloon_Button_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Balloon_Button.BackColor = Color.FromName("Red");
                c_EL.BalloonElement.bBrush = true;
            }
        }

        // Pogo Ability Button
        private void button15_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                PU_Pogo_Button.BackColor = Color.FromName("Red");
                c_EL.PogoElement.bBrush = true;
            }
        }

        // Cheetah Ability Button
        private void button14_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                PU_Cheetah_Button.BackColor = Color.FromName("Red");
                c_EL.CheetahElement.bBrush = true;
            }
        }

        // Fizzy Ability Button
        private void button16_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Fizzy_Drink_Button.BackColor = Color.FromName("Red");
                c_EL.FizzyElement.bBrush = true;
            }
        }

        // Greased Pig Ability Button
        private void button18_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                Greased_Pig_Button.BackColor = Color.FromName("Red");
                c_EL.GreasedPigElement.bBrush = true;
            }
        }

        // Flubber Ability Button
        private void button17_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                {
                    bCollisionBrush = false;
                }

                Flubber_Button.BackColor = Color.FromName("Red");
                c_EL.FlubbElement.bBrush = true;
            }
        }

        // Water Gun Button
        private void button21_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush == true)
                    bCollisionBrush = false;

                button21.BackColor = Color.FromName("Red");
                c_EL.WaterGunElement.bBrush = true;
            }
        }

        // Cotton Candy Launcher Button
        private void button20_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                button20.BackColor = Color.FromName("Red");
                c_EL.CottonCandyElement.bBrush = true;
            }
        }

        // Balloon Gun Button
        private void button19_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                button19.BackColor = Color.FromName("Red");
                c_EL.BalloonGunElement.bBrush = true;
            }
        }

        // Rat Button
        private void button5_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                Rat_Button.BackColor = Color.FromName("Red");
                c_EL.RatElement.bBrush = true;
            }
        }

        // Clown Button
        private void button6_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                Clown_Button.BackColor = Color.FromName("Red");
                c_EL.ClownElement.bBrush = true;
            }
        }

        // Hoodlum Button
        private void button4_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                Hoodlum_Button.BackColor = Color.FromName("Red");
                c_EL.HoodlumElement.bBrush = true;
            }
        }

        // Corinne Button
        private void button12_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                button12.BackColor = Color.FromName("Red");
                c_EL.CorinneElement.bBrush = true;
                
            }
        }

        // Fox Button
        private void button13_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                button13.BackColor = Color.FromName("Red");
                c_EL.FoxElement.bBrush = true;
            }
        }

        // Checkpoint Button
        private void button3_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;
                button3.BackColor = Color.FromName("Red");
                c_EL.CheckpointElement.bBrush = true;
            }
        }

        // Gravity Flip Button
        private void button1_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                button1.BackColor = Color.FromName("Red");
                bGravityFlipBrush = true;

            }
        }

        // Jump Node Button
        private void button22_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                Jump_Node.BackColor = Color.FromName("Red");
                bJumpNodeBrush = true;

            }
        }

        // Stop Node Button
        private void Stop_Node_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;

                Stop_Node.BackColor = Color.FromName("Red");
                bStopNodeBrush = true;

            }
        }

        private void saveWorldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();

            dlg.Filter = "All Files |*.*| Xml Files | *.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement root = new XElement("Level");
                
                // Replace 1 with Level Name Indicator
                // Make form for it <--
                // Adding World Background Object to XML
                root.Add( new XAttribute( "Name", 1 ) );

                root.Add(new XAttribute("World_Width", mapSize.Width));
                root.Add(new XAttribute("End_Point_X", (winPoint.X * tileSize.Width )));

                if (c_EL.BackgroundIMG.nImageID != -1)
                {
                    root.Add(new XAttribute("BG_Width", c_EL.BackgroundIMG.sImageSize.Width));
                    root.Add(new XAttribute("BG_Height", c_EL.BackgroundIMG.sImageSize.Height));
                    root.Add(new XAttribute("BG_RectWidth", c_EL.BackgroundIMG.rIRect.Width));
                    root.Add(new XAttribute("BG_RectHeight", c_EL.BackgroundIMG.rIRect.Height));
                    root.Add(new XAttribute("BG_ImageID", c_EL.BackgroundIMG.nImageID));
                    root.Add(new XAttribute("BG_LayerType", c_EL.BackgroundIMG.nLayerType));
                    root.Add(new XAttribute("BG_Filename", "MASQ_" + c_EL.BackgroundIMG.szFilename)); // Check filepath saving for this Images. Maybe make a Image folder or direct user to Resources folder for our project.
                }

                // Adding Object Elements to XML
                XElement uObj = new XElement("Unique_Objects");
                for (int x = 0; x < mapSize.Width; ++x)
                {
                    for (int y = 0; y < mapSize.Height; ++y)
                    {
                        if (c_TM.szEventID[x, y] != null)
                        {
                            XElement Obj = new XElement("Object");
                            Obj.Add(new XAttribute("EventID", c_TM.szEventID[x, y]));
                            Obj.Add(new XAttribute("X", x * tileSize.Width ) );
                            Obj.Add(new XAttribute("Y", y * tileSize.Height));

                            if (c_TM.szEventID[x, y].Contains("Vert_Gate"))
                            {
                                Obj.Add(new XAttribute("Trigger_EventID", c_TM.szTriggerEventID[x, y]));
                            }

                            if (c_TM.szEventID[x, y].Contains("Horz_Gate"))
                            {
                                Obj.Add(new XAttribute("Trigger_EventID", c_TM.szTriggerEventID[x, y]));
                            }    

                            if (c_TM.szEventID[x, y].Contains("Rat") )
                                // Saving all Rats Patrol Distance to same values
                                Obj.Add(new XAttribute( "Patrol_Distance", c_EL.RatElement.nPatrolDistance ));

                            if (c_TM.szEventID[x, y].Contains("Hoodlum") )
                                // Saving all Hoodlums Patrol Distance to same values
                                Obj.Add(new XAttribute( "Patrol_Distance", c_EL.HoodlumElement.nPatrolDistance ));

                            if (c_TM.szEventID[x, y].Contains("Cheetah"))
                                Obj.Add(new XAttribute("Powerup_Type", (int)PowerupType.CHEETAH));

                            if (c_TM.szEventID[x, y].Contains("Fizzy"))
                                Obj.Add(new XAttribute("Powerup_Type", (int)PowerupType.FIZZY));

                            if (c_TM.szEventID[x, y].Contains("Greased_Pig"))
                                Obj.Add(new XAttribute("Powerup_Type", (int)PowerupType.GREASED_PIG));

                            if (c_TM.szEventID[x, y].Contains("Flubb"))
                                Obj.Add(new XAttribute("Powerup_Type", (int)PowerupType.FLUBBER));

                            if (c_TM.szEventID[x, y].Contains("Pogo"))
                                Obj.Add(new XAttribute("Powerup_Type", (int)PowerupType.POGO));

                            if (c_TM.szEventID[x, y].Contains("Tool_Water_Gun_"))
                                Obj.Add(new XAttribute("Tool_Type", (int)ToolType.SQUIRT));

                            if (c_TM.szEventID[x, y].Contains("Tool_Cotton_Candy_Gun_"))
                                Obj.Add(new XAttribute("Tool_Type", (int)ToolType.COTTON_CANDY));

                            if (c_TM.szEventID[x, y].Contains("Tool_Loon_Gun"))
                                Obj.Add(new XAttribute("Tool_Type", (int)ToolType.BALLOON));

                            uObj.Add(Obj);
                        }
                    }
                }
                root.Add(uObj);

                // Tile World Creation to XML
                XElement ut = new XElement("Unique_Tiles");
                ut.Add(new XAttribute("IMGFileName", IMGfilename));
                ut.Add(new XAttribute("Image_Width", rTileRect.Width));
                ut.Add(new XAttribute("Image_Height", rTileRect.Height));
                ut.Add(new XAttribute( "Tile_Widths", tileSize.Width ) );
                ut.Add(new XAttribute( "Tile_Heights", tileSize.Height ) );
                
                // Loop through map and define Tile data

                // Adding Collide and Play Layer Tiles to XML
                for (int x = 0; x < mapSize.Width; ++x)
                {
                    for (int y = 0; y < mapSize.Height; ++y)
                    {

                        if (c_TM.nLayerType[x, y] == (int)LayerType.PLAY_LAYER)
                        {
                            XElement t = new XElement("Tile");
                            t.Add(new XAttribute("Layer_Type", (int)LayerType.PLAY_LAYER));
                            t.Add(new XAttribute("TileX", x));
                            t.Add(new XAttribute("TileY", y));
                            t.Add(new XAttribute("X", x * tileSize.Width));
                            t.Add(new XAttribute("Y", y * tileSize.Height));
                            t.Add(new XAttribute("RectX", c_TM.rPlayRect[x, y].X));
                            t.Add(new XAttribute("RectY", c_TM.rPlayRect[x, y].Y));
                            t.Add(new XAttribute("RectWidth", c_TM.rPlayRect[x, y].Width));
                            t.Add(new XAttribute("RectHeight", c_TM.rPlayRect[x, y].Height));
                            
                            // Checking to see if a Tile is on the Collision layer
                            if (c_TM.nCollideType[x, y] == (int)CollideType.COLLIDE)
                            {
                                t.Add(new XAttribute("Collide_Type", (int)CollideType.COLLIDE));
                                t.Add(new XAttribute("ImageID", c_TM.nPlayTileImageID[x, y]));
                                ut.Add(t);
                            }
                            else if(c_TM.nCollideType[x, y] == (int)CollideType.NONCOLLIDE)
                            {
                                t.Add(new XAttribute("Collide_Type", -1 ));
                                t.Add(new XAttribute("ImageID", c_TM.nPlayTileImageID[x, y]));
                                ut.Add(t);
                            }
                            else if (c_TM.nCollideType[x, y] == (int)CollideType.JUMP)
                            {
                                t.Add(new XAttribute("Collide_Type", (int)CollideType.JUMP));
                                t.Add(new XAttribute("ImageID", c_TM.nPlayTileImageID[x, y]));
                                ut.Add(t);
                            }
                            else if (c_TM.nCollideType[x, y] == (int)CollideType.STOP)
                            {
                                t.Add(new XAttribute("Collide_Type", (int)CollideType.STOP));
                                t.Add(new XAttribute("ImageID", c_TM.nPlayTileImageID[x, y]));
                                ut.Add(t);
                            }
                            else if (c_TM.nCollideType[x, y] == (int)CollideType.GRAVITYON)
                            {
                                t.Add(new XAttribute("Collide_Type", (int)CollideType.GRAVITYON));
                                t.Add(new XAttribute("ImageID", c_TM.nPlayTileImageID[x, y]));
                                ut.Add(t);
                            }
                        }

                        
                    }
                }


                // Adding Front Layer Tiles to XML
                for (int x = 0; x < mapSize.Width; ++x)
                {
                    for (int y = 0; y < mapSize.Height; ++y)
                    {
                        // Checking to see if a Tile is on Front layer
                        if (c_TM.nLayerType[x, y] == (int)LayerType.FRONT_LAYER)
                        {
                            XElement t = new XElement("Tile");
                            t.Add(new XAttribute("TileX", x));
                            t.Add(new XAttribute("TileY", y));
                            t.Add(new XAttribute("X", x * tileSize.Width ));
                            t.Add(new XAttribute("Y", y * tileSize.Height ));
                            t.Add(new XAttribute("RectX", c_TM.rFrontRect[x, y].X));
                            t.Add(new XAttribute("RectY", c_TM.rFrontRect[x, y].Y));
                            t.Add(new XAttribute("RectWidth", c_TM.rFrontRect[x, y].Width));
                            t.Add(new XAttribute("RectHeight", c_TM.rFrontRect[x, y].Height));
                            t.Add(new XAttribute("Layer_Type", (int)LayerType.FRONT_LAYER));
                            t.Add(new XAttribute("Collide_Type", -1));
                            t.Add(new XAttribute("ImageID", c_TM.nFrontTileImageID[x, y]));
                            ut.Add(t);
                        }
                    }
                }

                // Adding Mid Background Layer Tiles to XML
                for (int x = 0; x < mapSize.Width; ++x)
                {
                    for (int y = 0; y < mapSize.Height; ++y)
                    {
                        // Checking to see if a Tile is on Mid-Back layer
                        if (c_TM.nLayerType[x, y] == (int)LayerType.MID_LAYER)
                        {
                            XElement t = new XElement("Tile");
                            t.Add(new XAttribute("TileX", x));
                            t.Add(new XAttribute("TileY", y));
                            t.Add(new XAttribute("X", x * tileSize.Width ));
                            t.Add(new XAttribute("Y", y * tileSize.Height ));
                            t.Add(new XAttribute("RectX", c_TM.rMidBackRect[x, y].X));
                            t.Add(new XAttribute("RectY", c_TM.rMidBackRect[x, y].Y));
                            t.Add(new XAttribute("RectWidth", c_TM.rMidBackRect[x, y].Width));
                            t.Add(new XAttribute("RectHeight", c_TM.rMidBackRect[x, y].Height));
                            t.Add(new XAttribute("Layer_Type", (int)LayerType.MID_LAYER));
                            t.Add(new XAttribute("Collide_Type", -1));
                            t.Add(new XAttribute("ImageID", c_TM.nMidBackTileImageID[x, y]));

                            ut.Add(t);
                        }
                    }
                }

                root.Add(ut);

                root.Save(dlg.FileName);
            }
        }

        // File Load Button ////////////////////////////////////////////////////////////////////////////
        private void loadWorldToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = currentPath;
            dlg.Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                currentPath = Path.GetDirectoryName(dlg.FileName);
                LoadXML(dlg.FileName);
            }
        }

        // LoadXML Document ////////////////////////////////////////////////////////////////////////////
        private void LoadXML( string path )
        {
            XElement root = XElement.Load(path);

            // Loop through and reset values real quick
            // <--
            // <--
            // <--

            // Loading in Background
            //if (Convert.ToInt32(root.Attribute("BG_ImageID").Value) != -1 ) 
            //{
            //    // Filename path might be messed. Include woss for filepaths
            //    c_EL.BackgroundIMG.szFilename = root.Attribute("BG_Filename").Value;
            //    c_EL.BackgroundIMG.nImageID = mTM.LoadTexture(c_EL.BackgroundIMG.szFilename, 0);
            //    c_EL.BackgroundIMG.sImageSize = new Size(mTM.GetTextureWidth(c_EL.BackgroundIMG.nImageID), 
            //                                                mTM.GetTextureHeight(c_EL.BackgroundIMG.nImageID));
            //    c_EL.BackgroundIMG.rIRect = new Rectangle(new Point(0, 0), new Size(800, 600));

            //    // Begin and Back layers resets?
            //}

            
            // Loading in total World width in Tile Size
            mapSize.Width = Convert.ToInt32(root.Attribute("World_Width").Value);
            winPoint.X = (Convert.ToInt32(root.Attribute("End_Point_X").Value)) / 32;
            
            c_TM.map = new Point[mapSize.Width, mapSize.Height];
            c_TM.rPlayRect = new Rectangle[mapSize.Width, mapSize.Height];
            c_TM.rFrontRect = new Rectangle[mapSize.Width, mapSize.Height];
            c_TM.rMidBackRect = new Rectangle[mapSize.Width, mapSize.Height];

            c_TM.nPlayTileImageID = new int[mapSize.Width, mapSize.Height];
            c_TM.nMidBackTileImageID = new int[mapSize.Width, mapSize.Height];
            c_TM.nFrontTileImageID = new int[mapSize.Width, mapSize.Height];

            c_TM.nCollideType = new int[mapSize.Width, mapSize.Height];
            c_TM.nLayerType = new int[mapSize.Width, mapSize.Height];
            c_TM.szEventID = new string[mapSize.Width, mapSize.Height];
            c_TM.szTriggerEventID = new string[mapSize.Width, mapSize.Height];

            for (int x = 0; x < mapSize.Width; ++x)
            {
                for (int y = 0; y < mapSize.Height; ++y)
                {
                    c_TM.nPlayTileImageID[x, y] = -1;
                    c_TM.nMidBackTileImageID[x, y] = -1;
                    c_TM.nFrontTileImageID[x, y] = -1;

                    c_TM.nCollideType[x, y] = -1;

                    c_TM.nLayerType[x, y] = -1;
                    c_TM.szEventID[x, y] = null;
                    c_TM.szTriggerEventID[x, y] = null;


                }
            }
            
            // Loading in all Unique Objects
            XElement uo = root.Element("Unique_Objects");
            IEnumerable<XElement> objects = uo.Elements();
            int oX, oY = 0;

            foreach (XElement o in objects)
            {
                oX = (Convert.ToInt32(o.Attribute("X").Value) / 32 );
                oY = (Convert.ToInt32(o.Attribute("Y").Value) / 32 );

                if (o.Attribute("EventID").Value != null )
                {
                    if (o.Attribute("EventID").Value.Contains("Lever_"))
                        c_TM.szEventID[oX, oY ] = o.Attribute("EventID").Value;
                    else if (o.Attribute("EventID").Value.Contains("Gate_"))
                        c_TM.szEventID[oX, oY ] = o.Attribute("EventID").Value;
                    else
                        c_TM.szEventID[oX, oY ] = o.Attribute("EventID").Value;

                }

                if (o.Attribute("EventID").Value.Contains("Horz_Gate_") )
                {
                    c_TM.szTriggerEventID[oX, oY ] = o.Attribute("Trigger_EventID").Value;
                }

                if (o.Attribute("EventID").Value.Contains("Vert_Gate_") )
                {
                    c_TM.szTriggerEventID[oX , oY ] = o.Attribute("Trigger_EventID").Value;
                }
                //if (o.Attribute("EventID").Value == )
                //{
                    
                //}
            }

            // <--
            //
            XElement ut = root.Element("Unique_Tiles");
            
            IEnumerable<XElement> tiles = ut.Elements();
            int tX, tY = 0;

            foreach (XElement t in tiles)
            {
                tX = Convert.ToInt32(t.Attribute("TileX").Value);
                tY = Convert.ToInt32(t.Attribute("TileY").Value);

                // Checking XML Play Layer Tiles /////////////////////////////////////////////////////////
                if (Convert.ToInt32(t.Attribute("Layer_Type").Value) == (int)LayerType.PLAY_LAYER)
                {
                    c_TM.nLayerType[tX, tY] = Convert.ToInt32(t.Attribute("Layer_Type").Value);
                    c_TM.rPlayRect[tX, tY].X = Convert.ToInt32(t.Attribute("RectX").Value);
                    c_TM.rPlayRect[tX, tY].Y = Convert.ToInt32(t.Attribute("RectY").Value);
                    c_TM.rPlayRect[tX, tY].Size = new Size(Convert.ToInt32(t.Attribute("RectWidth").Value), Convert.ToInt32(t.Attribute("RectHeight").Value));

                    if (Convert.ToInt32(t.Attribute("Collide_Type").Value) == (int)CollideType.COLLIDE)
                    {
                        c_TM.nCollideType[tX, tY] = (int)CollideType.COLLIDE;
                        c_TM.nPlayTileImageID[tX, tY] = nTileSetID;
                    }
                    else if (Convert.ToInt32(t.Attribute("Collide_Type").Value) == (int)CollideType.GRAVITYON)
                    {
                        c_TM.nCollideType[tX, tY] = (int)CollideType.GRAVITYON;
                        if( Convert.ToInt32(t.Attribute("ImageID").Value) != -1 )
                            c_TM.nPlayTileImageID[tX, tY] = nTileSetID;
                    }
                    else if (Convert.ToInt32(t.Attribute("Collide_Type").Value) == (int)CollideType.JUMP)
                    {
                        c_TM.nCollideType[tX, tY] = (int)CollideType.JUMP;
                        if (Convert.ToInt32(t.Attribute("ImageID").Value) != -1)
                            c_TM.nPlayTileImageID[tX, tY] = nTileSetID;
                    }
                    else if (Convert.ToInt32(t.Attribute("Collide_Type").Value) == (int)CollideType.STOP)
                    {
                        c_TM.nCollideType[tX, tY] = (int)CollideType.STOP;
                        if (Convert.ToInt32(t.Attribute("ImageID").Value) != -1)
                            c_TM.nPlayTileImageID[tX, tY] = nTileSetID;
                    }
                    else
                    {
                        c_TM.nCollideType[tX, tY] = (int)CollideType.NONCOLLIDE;
                        if (Convert.ToInt32(t.Attribute("ImageID").Value) != -1)
                            c_TM.nPlayTileImageID[tX, tY] = nTileSetID;
                    }
                }

                // Checking XML Front Layer Tiles /////////////////////////////////////////////////////////
                else if (Convert.ToInt32(t.Attribute("Layer_Type").Value) == (int)LayerType.FRONT_LAYER)
                {
                    c_TM.nLayerType[tX, tY] = Convert.ToInt32(t.Attribute("Layer_Type").Value);
                    c_TM.nCollideType[tX, tY] = -1;
                    c_TM.rFrontRect[tX, tY].X = Convert.ToInt32(t.Attribute("RectX").Value);
                    c_TM.rFrontRect[tX, tY].Y = Convert.ToInt32(t.Attribute("RectY").Value);
                    c_TM.rFrontRect[tX, tY].Size = new Size(Convert.ToInt32(t.Attribute("RectWidth").Value), Convert.ToInt32(t.Attribute("RectHeight").Value));
                    c_TM.nFrontTileImageID[tX, tY] = nTileSetID;
                    
                }

                // Checking XML Mid Layer Tiles /////////////////////////////////////////////////////////
                else if (Convert.ToInt32(t.Attribute("Layer_Type").Value) == (int)LayerType.MID_LAYER )
                {
                    c_TM.nCollideType[tX, tY] = -1;
                    c_TM.nLayerType[tX, tY] = Convert.ToInt32(t.Attribute("Layer_Type").Value);
                    c_TM.rMidBackRect[tX, tY].X = Convert.ToInt32(t.Attribute("RectX").Value);
                    c_TM.rMidBackRect[tX, tY].Y = Convert.ToInt32(t.Attribute("RectY").Value);
                    c_TM.rMidBackRect[tX, tY].Size = new Size(Convert.ToInt32(t.Attribute("RectWidth").Value), Convert.ToInt32(t.Attribute("RectHeight").Value));
                    c_TM.nMidBackTileImageID[tX, tY] = nTileSetID;
                }
            }

            // Reset Scrolling bars
            ScrollingSizeSet();
            bPlayLayer = true;
            bMidLayer = true;
            bFrontLayer = true;
            Front_Layer.Checked = true;
            Mid_Back_Layer.Checked = true;
            Play_Layer.Checked = true;
            

            bBeginLayer = false;

        }

        // Application Close ///////////////////////////////////////////////////////////////////////////
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
            mD3D.Shutdown();
            mTM.ShutdownManagedTextureManager();
        }

        // Tile Brush Button ///////////////////////////////////////////////////////////////////////////
        private void Tile_Brush_Click(object sender, EventArgs e)
        {
            
        }

        private void Play_Brush_CheckedChanged(object sender, EventArgs e)
        {
            bPlayBrush = true;

            bFrontBrush = false;
            bMidBrush = false;

            bStopNodeBrush = false;
            bEraseTileBrush = false;
            bEraseElementBrush = false;
            bEraseTileBrush = false;
        }

        private void Front_Brush_CheckedChanged(object sender, EventArgs e)
        {
            bFrontBrush = true;

            bPlayBrush = false;
            bMidBrush = false;

            bStopNodeBrush = false;
            bEraseTileBrush = false;
            bEraseElementBrush = false;
            bEraseTileBrush = false;
        }

        private void Mid_Back_Brush_CheckedChanged(object sender, EventArgs e)
        {
            bMidBrush = true;

            bFrontBrush = false;
            bPlayBrush = false;
            bStopNodeBrush = false;
            bEraseTileBrush = false;
            bEraseElementBrush = false;
        }

        private void Load_Image_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                // Load custom Image to place in.
                //c_EL.BackgroundIMG.nImageID = mTM.LoadTexture(dlg.FileName, 0);
                //c_EL.BackgroundIMG.sImageSize = new Size(mTM.GetTextureWidth(c_EL.BackgroundIMG.nImageID), mTM.GetTextureHeight(c_EL.BackgroundIMG.nImageID));
                //c_EL.BackgroundIMG.rIRect = new Rectangle(new Point(0, 0), new Size(800, 600));
            }
        }

        private void Tile_Eraser_Click(object sender, EventArgs e)
        {
            if (Play_Layer.Checked != false)
            {
                if (bCollisionBrush)
                    bCollisionBrush = false;
            }

            Tile_Eraser.BackColor = Color.FromName("Red");
            bEraseTileBrush = !bEraseTileBrush;

            if (bEraseTileBrush != true)
            {
                Tile_Eraser.BackColor = Color.FromName("Transparent");

            }

            
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {

            if (e.KeyCode == Keys.Enter)
            {
                Tile_World_Panel.HorizontalScroll.Value += 224;
            }
        }


        private void Tile_World_Panel_Scroll(object sender, ScrollEventArgs e)
        {
            if (e.ScrollOrientation == ScrollOrientation.VerticalScroll)
                Tile_World_Panel.HorizontalScroll.Value += 224;
        }

        private void Form1_KeyUp(object sender, KeyEventArgs e)
        {
            //if (e.KeyCode == Keys.D)
            //    Tile_World_Panel.HorizontalScroll.Value += 224;

            //if (e.KeyCode == Keys.A)
            //    Tile_World_Panel.HorizontalScroll.Value -= 224;
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

            bGateOpen = true;
        }


    }
}
