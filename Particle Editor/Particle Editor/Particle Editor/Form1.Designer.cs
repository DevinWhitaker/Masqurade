namespace Particle_Editor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openXMLToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openXMLAndAppendToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.randomizeCurrentEmitterToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lbEmitters = new System.Windows.Forms.ListBox();
            this.lbParticles = new System.Windows.Forms.ListBox();
            this.lbColors = new System.Windows.Forms.ListBox();
            this.lbSizes = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lParticleList = new System.Windows.Forms.Label();
            this.lNote = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.lColorCollection = new System.Windows.Forms.Label();
            this.gbVelStop = new System.Windows.Forms.GroupBox();
            this.label27 = new System.Windows.Forms.Label();
            this.label30 = new System.Windows.Forms.Label();
            this.trackBarVelocityTime = new System.Windows.Forms.TrackBar();
            this.nuYVelocity = new System.Windows.Forms.NumericUpDown();
            this.nuXVelocity = new System.Windows.Forms.NumericUpDown();
            this.lSizeCollection = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.bAddTenParticles = new System.Windows.Forms.Button();
            this.bRemoveParticle = new System.Windows.Forms.Button();
            this.bSelectAllParticles = new System.Windows.Forms.Button();
            this.bAddParticle = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.bRemoveVelocity = new System.Windows.Forms.Button();
            this.bAddVelocity = new System.Windows.Forms.Button();
            this.lVelocityCollection = new System.Windows.Forms.Label();
            this.lbVelocities = new System.Windows.Forms.ListBox();
            this.bRemoveColor = new System.Windows.Forms.Button();
            this.tbFilepath = new System.Windows.Forms.TextBox();
            this.bAddColor = new System.Windows.Forms.Button();
            this.bRemoveSize = new System.Windows.Forms.Button();
            this.bAddSize = new System.Windows.Forms.Button();
            this.gbSizeStop = new System.Windows.Forms.GroupBox();
            this.label9 = new System.Windows.Forms.Label();
            this.nuSizeWidth = new System.Windows.Forms.NumericUpDown();
            this.label12 = new System.Windows.Forms.Label();
            this.nuSizeHeight = new System.Windows.Forms.NumericUpDown();
            this.trackBarSizeTime = new System.Windows.Forms.TrackBar();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.gbColorStop = new System.Windows.Forms.GroupBox();
            this.label32 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.bChangeColor = new System.Windows.Forms.Button();
            this.label33 = new System.Windows.Forms.Label();
            this.trackBarColorTime = new System.Windows.Forms.TrackBar();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.cbBurst = new System.Windows.Forms.CheckBox();
            this.groupBox9 = new System.Windows.Forms.GroupBox();
            this.label29 = new System.Windows.Forms.Label();
            this.cbBlendDestination = new System.Windows.Forms.ComboBox();
            this.label25 = new System.Windows.Forms.Label();
            this.cbBlendSource = new System.Windows.Forms.ComboBox();
            this.bRemoveEmitter = new System.Windows.Forms.Button();
            this.cbLooping = new System.Windows.Forms.CheckBox();
            this.label23 = new System.Windows.Forms.Label();
            this.bLoadTexture = new System.Windows.Forms.Button();
            this.bAddEmitter = new System.Windows.Forms.Button();
            this.label22 = new System.Windows.Forms.Label();
            this.tbName = new System.Windows.Forms.TextBox();
            this.groupBox8 = new System.Windows.Forms.GroupBox();
            this.nuHeight = new System.Windows.Forms.NumericUpDown();
            this.nuWidth = new System.Windows.Forms.NumericUpDown();
            this.nuRadius = new System.Windows.Forms.NumericUpDown();
            this.labelHeight = new System.Windows.Forms.Label();
            this.labelWidth = new System.Windows.Forms.Label();
            this.labelRadius = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.cbShape = new System.Windows.Forms.ComboBox();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.nuExternalYVelocity = new System.Windows.Forms.NumericUpDown();
            this.nuExternalXVelocity = new System.Windows.Forms.NumericUpDown();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.nuParticleDuration = new System.Windows.Forms.NumericUpDown();
            this.nuNumParticles = new System.Windows.Forms.NumericUpDown();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.nuEmitterDuration = new System.Windows.Forms.NumericUpDown();
            this.label13 = new System.Windows.Forms.Label();
            this.pTexture = new System.Windows.Forms.Panel();
            this.pPreview = new System.Windows.Forms.Panel();
            this.label24 = new System.Windows.Forms.Label();
            this.bRandom = new System.Windows.Forms.Button();
            this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
            this.cbBurstRand = new System.Windows.Forms.CheckBox();
            this.cbLoopingRand = new System.Windows.Forms.CheckBox();
            this.cbNumParticlesRand = new System.Windows.Forms.CheckBox();
            this.cbExternalForceRand = new System.Windows.Forms.CheckBox();
            this.cbEmitterDuration = new System.Windows.Forms.CheckBox();
            this.cbParticleDurationRand = new System.Windows.Forms.CheckBox();
            this.cbShapeRand = new System.Windows.Forms.CheckBox();
            this.cbScaleRand = new System.Windows.Forms.CheckBox();
            this.cbBlendModeRand = new System.Windows.Forms.CheckBox();
            this.cbScaleCollectionNumRand = new System.Windows.Forms.CheckBox();
            this.cbScaleStopNumRand = new System.Windows.Forms.CheckBox();
            this.groupBox10 = new System.Windows.Forms.GroupBox();
            this.bCheckAllRand = new System.Windows.Forms.Button();
            this.bUnCheckAllRand = new System.Windows.Forms.Button();
            this.groupBox15 = new System.Windows.Forms.GroupBox();
            this.groupBox14 = new System.Windows.Forms.GroupBox();
            this.cbVelocityCollectionNumRand = new System.Windows.Forms.CheckBox();
            this.cbVelocityStopNumRand = new System.Windows.Forms.CheckBox();
            this.cbVelocityCoefRand = new System.Windows.Forms.CheckBox();
            this.cbVelocityRand = new System.Windows.Forms.CheckBox();
            this.groupBox13 = new System.Windows.Forms.GroupBox();
            this.cbColorCollectionNumRand = new System.Windows.Forms.CheckBox();
            this.cbColorStopNumRand = new System.Windows.Forms.CheckBox();
            this.cbColorCoefRand = new System.Windows.Forms.CheckBox();
            this.cbColorRand = new System.Windows.Forms.CheckBox();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.cbScaleCoefRand = new System.Windows.Forms.CheckBox();
            this.groupBox11 = new System.Windows.Forms.GroupBox();
            this.cbAnchorToBG = new System.Windows.Forms.CheckBox();
            this.bLoadBG = new System.Windows.Forms.Button();
            this.checkBox9 = new System.Windows.Forms.CheckBox();
            this.nuScale = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.menuStrip1.SuspendLayout();
            this.gbVelStop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVelocityTime)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuYVelocity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuXVelocity)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.gbSizeStop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuSizeWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuSizeHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSizeTime)).BeginInit();
            this.gbColorStop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarColorTime)).BeginInit();
            this.groupBox3.SuspendLayout();
            this.groupBox9.SuspendLayout();
            this.groupBox8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuRadius)).BeginInit();
            this.groupBox7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuExternalYVelocity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuExternalXVelocity)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuParticleDuration)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuNumParticles)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuEmitterDuration)).BeginInit();
            this.groupBox10.SuspendLayout();
            this.groupBox15.SuspendLayout();
            this.groupBox14.SuspendLayout();
            this.groupBox13.SuspendLayout();
            this.groupBox12.SuspendLayout();
            this.groupBox11.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuScale)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1279, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.openXMLToolStripMenuItem,
            this.openXMLAndAppendToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.openToolStripMenuItem.Text = "Load Texture";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // openXMLToolStripMenuItem
            // 
            this.openXMLToolStripMenuItem.Name = "openXMLToolStripMenuItem";
            this.openXMLToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.openXMLToolStripMenuItem.Text = "Open XML";
            this.openXMLToolStripMenuItem.Click += new System.EventHandler(this.openXMLToolStripMenuItem_Click);
            // 
            // openXMLAndAppendToolStripMenuItem
            // 
            this.openXMLAndAppendToolStripMenuItem.Name = "openXMLAndAppendToolStripMenuItem";
            this.openXMLAndAppendToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.openXMLAndAppendToolStripMenuItem.Text = "Open XML and Append";
            this.openXMLAndAppendToolStripMenuItem.Click += new System.EventHandler(this.openXMLAndAppendToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.saveToolStripMenuItem.Text = "Save XML";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(198, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.randomizeCurrentEmitterToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // randomizeCurrentEmitterToolStripMenuItem
            // 
            this.randomizeCurrentEmitterToolStripMenuItem.Name = "randomizeCurrentEmitterToolStripMenuItem";
            this.randomizeCurrentEmitterToolStripMenuItem.Size = new System.Drawing.Size(217, 22);
            this.randomizeCurrentEmitterToolStripMenuItem.Text = "Randomize Current Emitter";
            this.randomizeCurrentEmitterToolStripMenuItem.Click += new System.EventHandler(this.randomizeCurrentEmitterToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // lbEmitters
            // 
            this.lbEmitters.FormattingEnabled = true;
            this.lbEmitters.Location = new System.Drawing.Point(16, 30);
            this.lbEmitters.Name = "lbEmitters";
            this.lbEmitters.Size = new System.Drawing.Size(177, 212);
            this.lbEmitters.TabIndex = 1;
            this.lbEmitters.SelectedIndexChanged += new System.EventHandler(this.lbEmitters_SelectedIndexChanged);
            // 
            // lbParticles
            // 
            this.lbParticles.FormattingEnabled = true;
            this.lbParticles.Location = new System.Drawing.Point(6, 35);
            this.lbParticles.Name = "lbParticles";
            this.lbParticles.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lbParticles.Size = new System.Drawing.Size(88, 511);
            this.lbParticles.TabIndex = 2;
            this.lbParticles.SelectedIndexChanged += new System.EventHandler(this.lbParticles_SelectedIndexChanged);
            // 
            // lbColors
            // 
            this.lbColors.FormattingEnabled = true;
            this.lbColors.Location = new System.Drawing.Point(11, 41);
            this.lbColors.Name = "lbColors";
            this.lbColors.Size = new System.Drawing.Size(66, 108);
            this.lbColors.TabIndex = 3;
            this.lbColors.SelectedIndexChanged += new System.EventHandler(this.lbColors_SelectedIndexChanged);
            // 
            // lbSizes
            // 
            this.lbSizes.FormattingEnabled = true;
            this.lbSizes.Location = new System.Drawing.Point(11, 212);
            this.lbSizes.Name = "lbSizes";
            this.lbSizes.Size = new System.Drawing.Size(66, 108);
            this.lbSizes.TabIndex = 4;
            this.lbSizes.SelectedIndexChanged += new System.EventHandler(this.lbSizes_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 14);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Emitters";
            // 
            // lParticleList
            // 
            this.lParticleList.AutoSize = true;
            this.lParticleList.Location = new System.Drawing.Point(15, 19);
            this.lParticleList.Name = "lParticleList";
            this.lParticleList.Size = new System.Drawing.Size(61, 13);
            this.lParticleList.TabIndex = 6;
            this.lParticleList.Text = "Particle List";
            // 
            // lNote
            // 
            this.lNote.AutoSize = true;
            this.lNote.Location = new System.Drawing.Point(86, 555);
            this.lNote.Name = "lNote";
            this.lNote.Size = new System.Drawing.Size(291, 13);
            this.lNote.TabIndex = 7;
            this.lNote.Text = "Note: Particles Will be emitted in order from lowest to highest";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 16);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(54, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "X Velocity";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(6, 38);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(54, 13);
            this.label5.TabIndex = 9;
            this.label5.Text = "Y Velocity";
            // 
            // lColorCollection
            // 
            this.lColorCollection.AutoSize = true;
            this.lColorCollection.Location = new System.Drawing.Point(8, 22);
            this.lColorCollection.Name = "lColorCollection";
            this.lColorCollection.Size = new System.Drawing.Size(80, 13);
            this.lColorCollection.TabIndex = 10;
            this.lColorCollection.Text = "Color Collection";
            // 
            // gbVelStop
            // 
            this.gbVelStop.Controls.Add(this.label27);
            this.gbVelStop.Controls.Add(this.label30);
            this.gbVelStop.Controls.Add(this.trackBarVelocityTime);
            this.gbVelStop.Controls.Add(this.nuYVelocity);
            this.gbVelStop.Controls.Add(this.nuXVelocity);
            this.gbVelStop.Controls.Add(this.label4);
            this.gbVelStop.Controls.Add(this.label5);
            this.gbVelStop.Location = new System.Drawing.Point(83, 380);
            this.gbVelStop.Name = "gbVelStop";
            this.gbVelStop.Size = new System.Drawing.Size(154, 108);
            this.gbVelStop.TabIndex = 11;
            this.gbVelStop.TabStop = false;
            this.gbVelStop.Text = "Velocity Stop";
            // 
            // label27
            // 
            this.label27.AutoSize = true;
            this.label27.Location = new System.Drawing.Point(111, 85);
            this.label27.Name = "label27";
            this.label27.Size = new System.Drawing.Size(36, 13);
            this.label27.TabIndex = 48;
            this.label27.Text = "Death";
            // 
            // label30
            // 
            this.label30.AutoSize = true;
            this.label30.Location = new System.Drawing.Point(8, 85);
            this.label30.Name = "label30";
            this.label30.Size = new System.Drawing.Size(28, 13);
            this.label30.TabIndex = 47;
            this.label30.Text = "Birth";
            // 
            // trackBarVelocityTime
            // 
            this.trackBarVelocityTime.Location = new System.Drawing.Point(6, 62);
            this.trackBarVelocityTime.Maximum = 100;
            this.trackBarVelocityTime.Name = "trackBarVelocityTime";
            this.trackBarVelocityTime.Size = new System.Drawing.Size(141, 45);
            this.trackBarVelocityTime.TabIndex = 46;
            this.trackBarVelocityTime.Tag = "";
            this.trackBarVelocityTime.MouseUp += new System.Windows.Forms.MouseEventHandler(this.trackBarVelocityTime_MouseUp);
            // 
            // nuYVelocity
            // 
            this.nuYVelocity.DecimalPlaces = 2;
            this.nuYVelocity.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuYVelocity.Location = new System.Drawing.Point(66, 36);
            this.nuYVelocity.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.nuYVelocity.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            -2147483648});
            this.nuYVelocity.Name = "nuYVelocity";
            this.nuYVelocity.Size = new System.Drawing.Size(75, 20);
            this.nuYVelocity.TabIndex = 16;
            this.nuYVelocity.ValueChanged += new System.EventHandler(this.nuYVelocity_ValueChanged);
            // 
            // nuXVelocity
            // 
            this.nuXVelocity.DecimalPlaces = 2;
            this.nuXVelocity.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuXVelocity.Location = new System.Drawing.Point(66, 14);
            this.nuXVelocity.Maximum = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.nuXVelocity.Minimum = new decimal(new int[] {
            500,
            0,
            0,
            -2147483648});
            this.nuXVelocity.Name = "nuXVelocity";
            this.nuXVelocity.Size = new System.Drawing.Size(75, 20);
            this.nuXVelocity.TabIndex = 15;
            this.nuXVelocity.ValueChanged += new System.EventHandler(this.nuXVelocity_ValueChanged);
            // 
            // lSizeCollection
            // 
            this.lSizeCollection.AutoSize = true;
            this.lSizeCollection.Location = new System.Drawing.Point(9, 193);
            this.lSizeCollection.Name = "lSizeCollection";
            this.lSizeCollection.Size = new System.Drawing.Size(76, 13);
            this.lSizeCollection.TabIndex = 12;
            this.lSizeCollection.Text = "Size Collection";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.bAddTenParticles);
            this.groupBox2.Controls.Add(this.bRemoveParticle);
            this.groupBox2.Controls.Add(this.bSelectAllParticles);
            this.groupBox2.Controls.Add(this.bAddParticle);
            this.groupBox2.Controls.Add(this.groupBox6);
            this.groupBox2.Controls.Add(this.lbParticles);
            this.groupBox2.Controls.Add(this.lNote);
            this.groupBox2.Controls.Add(this.lParticleList);
            this.groupBox2.Location = new System.Drawing.Point(249, 29);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(385, 605);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Particles";
            // 
            // bAddTenParticles
            // 
            this.bAddTenParticles.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddTenParticles.Location = new System.Drawing.Point(41, 550);
            this.bAddTenParticles.Name = "bAddTenParticles";
            this.bAddTenParticles.Size = new System.Drawing.Size(39, 23);
            this.bAddTenParticles.TabIndex = 36;
            this.bAddTenParticles.Text = "+10";
            this.bAddTenParticles.UseVisualStyleBackColor = true;
            this.bAddTenParticles.Click += new System.EventHandler(this.bAddTenParticles_Click);
            // 
            // bRemoveParticle
            // 
            this.bRemoveParticle.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRemoveParticle.Location = new System.Drawing.Point(6, 576);
            this.bRemoveParticle.Name = "bRemoveParticle";
            this.bRemoveParticle.Size = new System.Drawing.Size(29, 23);
            this.bRemoveParticle.TabIndex = 35;
            this.bRemoveParticle.Text = "-";
            this.bRemoveParticle.UseVisualStyleBackColor = true;
            this.bRemoveParticle.Click += new System.EventHandler(this.bRemoveParticle_Click);
            // 
            // bSelectAllParticles
            // 
            this.bSelectAllParticles.Location = new System.Drawing.Point(41, 576);
            this.bSelectAllParticles.Name = "bSelectAllParticles";
            this.bSelectAllParticles.Size = new System.Drawing.Size(62, 23);
            this.bSelectAllParticles.TabIndex = 17;
            this.bSelectAllParticles.Text = "Select All";
            this.bSelectAllParticles.UseVisualStyleBackColor = true;
            this.bSelectAllParticles.Click += new System.EventHandler(this.bSelectAllParticles_Click);
            // 
            // bAddParticle
            // 
            this.bAddParticle.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddParticle.Location = new System.Drawing.Point(6, 550);
            this.bAddParticle.Name = "bAddParticle";
            this.bAddParticle.Size = new System.Drawing.Size(29, 23);
            this.bAddParticle.TabIndex = 34;
            this.bAddParticle.Text = "+";
            this.bAddParticle.UseVisualStyleBackColor = true;
            this.bAddParticle.Click += new System.EventHandler(this.bAddParticle_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.bRemoveVelocity);
            this.groupBox6.Controls.Add(this.bAddVelocity);
            this.groupBox6.Controls.Add(this.lVelocityCollection);
            this.groupBox6.Controls.Add(this.lbVelocities);
            this.groupBox6.Controls.Add(this.bRemoveColor);
            this.groupBox6.Controls.Add(this.gbVelStop);
            this.groupBox6.Controls.Add(this.tbFilepath);
            this.groupBox6.Controls.Add(this.bAddColor);
            this.groupBox6.Controls.Add(this.bRemoveSize);
            this.groupBox6.Controls.Add(this.bAddSize);
            this.groupBox6.Controls.Add(this.gbSizeStop);
            this.groupBox6.Controls.Add(this.gbColorStop);
            this.groupBox6.Controls.Add(this.lSizeCollection);
            this.groupBox6.Controls.Add(this.lbColors);
            this.groupBox6.Controls.Add(this.lColorCollection);
            this.groupBox6.Controls.Add(this.lbSizes);
            this.groupBox6.Location = new System.Drawing.Point(100, 19);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(248, 527);
            this.groupBox6.TabIndex = 16;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Particle Details";
            // 
            // bRemoveVelocity
            // 
            this.bRemoveVelocity.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRemoveVelocity.Location = new System.Drawing.Point(44, 494);
            this.bRemoveVelocity.Name = "bRemoveVelocity";
            this.bRemoveVelocity.Size = new System.Drawing.Size(29, 23);
            this.bRemoveVelocity.TabIndex = 33;
            this.bRemoveVelocity.Text = "-";
            this.bRemoveVelocity.UseVisualStyleBackColor = true;
            this.bRemoveVelocity.Click += new System.EventHandler(this.bRemoveVelocity_Click);
            // 
            // bAddVelocity
            // 
            this.bAddVelocity.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddVelocity.Location = new System.Drawing.Point(11, 494);
            this.bAddVelocity.Name = "bAddVelocity";
            this.bAddVelocity.Size = new System.Drawing.Size(29, 23);
            this.bAddVelocity.TabIndex = 32;
            this.bAddVelocity.Text = "+";
            this.bAddVelocity.UseVisualStyleBackColor = true;
            this.bAddVelocity.Click += new System.EventHandler(this.bAddVelocity_Click);
            // 
            // lVelocityCollection
            // 
            this.lVelocityCollection.AutoSize = true;
            this.lVelocityCollection.Location = new System.Drawing.Point(9, 361);
            this.lVelocityCollection.Name = "lVelocityCollection";
            this.lVelocityCollection.Size = new System.Drawing.Size(93, 13);
            this.lVelocityCollection.TabIndex = 31;
            this.lVelocityCollection.Text = "Velocity Collection";
            // 
            // lbVelocities
            // 
            this.lbVelocities.FormattingEnabled = true;
            this.lbVelocities.Location = new System.Drawing.Point(11, 380);
            this.lbVelocities.Name = "lbVelocities";
            this.lbVelocities.Size = new System.Drawing.Size(66, 108);
            this.lbVelocities.TabIndex = 30;
            this.lbVelocities.SelectedIndexChanged += new System.EventHandler(this.lbVelocities_SelectedIndexChanged);
            // 
            // bRemoveColor
            // 
            this.bRemoveColor.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRemoveColor.Location = new System.Drawing.Point(44, 156);
            this.bRemoveColor.Name = "bRemoveColor";
            this.bRemoveColor.Size = new System.Drawing.Size(29, 23);
            this.bRemoveColor.TabIndex = 29;
            this.bRemoveColor.Text = "-";
            this.bRemoveColor.UseVisualStyleBackColor = true;
            this.bRemoveColor.Click += new System.EventHandler(this.bRemoveColor_Click);
            // 
            // tbFilepath
            // 
            this.tbFilepath.Location = new System.Drawing.Point(125, 159);
            this.tbFilepath.Name = "tbFilepath";
            this.tbFilepath.Size = new System.Drawing.Size(99, 20);
            this.tbFilepath.TabIndex = 17;
            this.tbFilepath.Visible = false;
            // 
            // bAddColor
            // 
            this.bAddColor.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddColor.Location = new System.Drawing.Point(11, 156);
            this.bAddColor.Name = "bAddColor";
            this.bAddColor.Size = new System.Drawing.Size(29, 23);
            this.bAddColor.TabIndex = 28;
            this.bAddColor.Text = "+";
            this.bAddColor.UseVisualStyleBackColor = true;
            this.bAddColor.Click += new System.EventHandler(this.bAddColor_Click);
            // 
            // bRemoveSize
            // 
            this.bRemoveSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRemoveSize.Location = new System.Drawing.Point(44, 326);
            this.bRemoveSize.Name = "bRemoveSize";
            this.bRemoveSize.Size = new System.Drawing.Size(29, 23);
            this.bRemoveSize.TabIndex = 27;
            this.bRemoveSize.Text = "-";
            this.bRemoveSize.UseVisualStyleBackColor = true;
            this.bRemoveSize.Click += new System.EventHandler(this.bRemoveSize_Click);
            // 
            // bAddSize
            // 
            this.bAddSize.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddSize.Location = new System.Drawing.Point(11, 326);
            this.bAddSize.Name = "bAddSize";
            this.bAddSize.Size = new System.Drawing.Size(29, 23);
            this.bAddSize.TabIndex = 26;
            this.bAddSize.Text = "+";
            this.bAddSize.UseVisualStyleBackColor = true;
            this.bAddSize.Click += new System.EventHandler(this.bAddSize_Click);
            // 
            // gbSizeStop
            // 
            this.gbSizeStop.Controls.Add(this.label9);
            this.gbSizeStop.Controls.Add(this.nuSizeWidth);
            this.gbSizeStop.Controls.Add(this.label12);
            this.gbSizeStop.Controls.Add(this.nuSizeHeight);
            this.gbSizeStop.Controls.Add(this.trackBarSizeTime);
            this.gbSizeStop.Controls.Add(this.label10);
            this.gbSizeStop.Controls.Add(this.label11);
            this.gbSizeStop.Location = new System.Drawing.Point(83, 212);
            this.gbSizeStop.Name = "gbSizeStop";
            this.gbSizeStop.Size = new System.Drawing.Size(154, 108);
            this.gbSizeStop.TabIndex = 15;
            this.gbSizeStop.TabStop = false;
            this.gbSizeStop.Text = "Size Stop";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(111, 85);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(36, 13);
            this.label9.TabIndex = 45;
            this.label9.Text = "Death";
            // 
            // nuSizeWidth
            // 
            this.nuSizeWidth.AccessibleName = "nuSizeWidth";
            this.nuSizeWidth.DecimalPlaces = 2;
            this.nuSizeWidth.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuSizeWidth.Location = new System.Drawing.Point(72, 14);
            this.nuSizeWidth.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nuSizeWidth.Name = "nuSizeWidth";
            this.nuSizeWidth.Size = new System.Drawing.Size(75, 20);
            this.nuSizeWidth.TabIndex = 16;
            this.nuSizeWidth.ValueChanged += new System.EventHandler(this.nuSizeWidth_ValueChanged);
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(8, 85);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(28, 13);
            this.label12.TabIndex = 44;
            this.label12.Text = "Birth";
            // 
            // nuSizeHeight
            // 
            this.nuSizeHeight.DecimalPlaces = 2;
            this.nuSizeHeight.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuSizeHeight.Location = new System.Drawing.Point(72, 39);
            this.nuSizeHeight.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.nuSizeHeight.Name = "nuSizeHeight";
            this.nuSizeHeight.Size = new System.Drawing.Size(75, 20);
            this.nuSizeHeight.TabIndex = 14;
            this.nuSizeHeight.ValueChanged += new System.EventHandler(this.nuSizeHeight_ValueChanged);
            // 
            // trackBarSizeTime
            // 
            this.trackBarSizeTime.Location = new System.Drawing.Point(6, 62);
            this.trackBarSizeTime.Maximum = 100;
            this.trackBarSizeTime.Name = "trackBarSizeTime";
            this.trackBarSizeTime.Size = new System.Drawing.Size(141, 45);
            this.trackBarSizeTime.TabIndex = 43;
            this.trackBarSizeTime.Tag = "";
            this.trackBarSizeTime.MouseUp += new System.Windows.Forms.MouseEventHandler(this.trackBarSizeTime_MouseUp);
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(6, 16);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(60, 13);
            this.label10.TabIndex = 8;
            this.label10.Text = "Width Coef";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(6, 41);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(63, 13);
            this.label11.TabIndex = 9;
            this.label11.Text = "Height Coef";
            // 
            // gbColorStop
            // 
            this.gbColorStop.Controls.Add(this.label32);
            this.gbColorStop.Controls.Add(this.label8);
            this.gbColorStop.Controls.Add(this.bChangeColor);
            this.gbColorStop.Controls.Add(this.label33);
            this.gbColorStop.Controls.Add(this.trackBarColorTime);
            this.gbColorStop.Location = new System.Drawing.Point(83, 41);
            this.gbColorStop.Name = "gbColorStop";
            this.gbColorStop.Size = new System.Drawing.Size(154, 84);
            this.gbColorStop.TabIndex = 12;
            this.gbColorStop.TabStop = false;
            this.gbColorStop.Text = "Color Stop";
            // 
            // label32
            // 
            this.label32.AutoSize = true;
            this.label32.Location = new System.Drawing.Point(111, 61);
            this.label32.Name = "label32";
            this.label32.Size = new System.Drawing.Size(36, 13);
            this.label32.TabIndex = 42;
            this.label32.Text = "Death";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(6, 16);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(31, 13);
            this.label8.TabIndex = 8;
            this.label8.Text = "Color";
            // 
            // bChangeColor
            // 
            this.bChangeColor.BackColor = System.Drawing.Color.White;
            this.bChangeColor.Location = new System.Drawing.Point(72, 12);
            this.bChangeColor.Name = "bChangeColor";
            this.bChangeColor.Size = new System.Drawing.Size(75, 20);
            this.bChangeColor.TabIndex = 13;
            this.bChangeColor.UseVisualStyleBackColor = false;
            this.bChangeColor.Click += new System.EventHandler(this.bChangeColor_Click);
            // 
            // label33
            // 
            this.label33.AutoSize = true;
            this.label33.Location = new System.Drawing.Point(8, 61);
            this.label33.Name = "label33";
            this.label33.Size = new System.Drawing.Size(28, 13);
            this.label33.TabIndex = 41;
            this.label33.Text = "Birth";
            // 
            // trackBarColorTime
            // 
            this.trackBarColorTime.Location = new System.Drawing.Point(6, 38);
            this.trackBarColorTime.Maximum = 100;
            this.trackBarColorTime.Name = "trackBarColorTime";
            this.trackBarColorTime.Size = new System.Drawing.Size(141, 45);
            this.trackBarColorTime.TabIndex = 40;
            this.trackBarColorTime.Tag = "";
            this.trackBarColorTime.Value = 1;
            this.trackBarColorTime.MouseUp += new System.Windows.Forms.MouseEventHandler(this.trackBarColorTime_MouseUp);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.nuScale);
            this.groupBox3.Controls.Add(this.cbBurst);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.groupBox9);
            this.groupBox3.Controls.Add(this.bRemoveEmitter);
            this.groupBox3.Controls.Add(this.cbLooping);
            this.groupBox3.Controls.Add(this.label23);
            this.groupBox3.Controls.Add(this.bLoadTexture);
            this.groupBox3.Controls.Add(this.bAddEmitter);
            this.groupBox3.Controls.Add(this.label22);
            this.groupBox3.Controls.Add(this.tbName);
            this.groupBox3.Controls.Add(this.groupBox8);
            this.groupBox3.Controls.Add(this.groupBox7);
            this.groupBox3.Controls.Add(this.nuParticleDuration);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.nuEmitterDuration);
            this.groupBox3.Controls.Add(this.label13);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.lbEmitters);
            this.groupBox3.Location = new System.Drawing.Point(10, 29);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(217, 756);
            this.groupBox3.TabIndex = 14;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Emitter";
            // 
            // cbBurst
            // 
            this.cbBurst.AutoSize = true;
            this.cbBurst.Location = new System.Drawing.Point(16, 335);
            this.cbBurst.Name = "cbBurst";
            this.cbBurst.Size = new System.Drawing.Size(50, 17);
            this.cbBurst.TabIndex = 27;
            this.cbBurst.Text = "Burst";
            this.cbBurst.UseVisualStyleBackColor = true;
            this.cbBurst.CheckedChanged += new System.EventHandler(this.cbBurst_CheckedChanged);
            // 
            // groupBox9
            // 
            this.groupBox9.Controls.Add(this.label29);
            this.groupBox9.Controls.Add(this.cbBlendDestination);
            this.groupBox9.Controls.Add(this.label25);
            this.groupBox9.Controls.Add(this.cbBlendSource);
            this.groupBox9.Location = new System.Drawing.Point(15, 513);
            this.groupBox9.Name = "groupBox9";
            this.groupBox9.Size = new System.Drawing.Size(174, 107);
            this.groupBox9.TabIndex = 29;
            this.groupBox9.TabStop = false;
            this.groupBox9.Text = "Blend Mode";
            // 
            // label29
            // 
            this.label29.AutoSize = true;
            this.label29.Location = new System.Drawing.Point(6, 59);
            this.label29.Name = "label29";
            this.label29.Size = new System.Drawing.Size(60, 13);
            this.label29.TabIndex = 28;
            this.label29.Text = "Destination";
            // 
            // cbBlendDestination
            // 
            this.cbBlendDestination.FormattingEnabled = true;
            this.cbBlendDestination.Items.AddRange(new object[] {
            "Zero",
            "One",
            "SourceColor",
            "InvSourceColor",
            "SourceAlpha",
            "InvSourceAlpha",
            "DestinationAlpha",
            "InvDestinationAlpha",
            "DestinationColor",
            "InvDestinationColor",
            "SourceAlphaSat",
            "BothSourceAlpha",
            "BothInvSourceAlpha",
            "BlendFactor",
            "InvBlendFactor"});
            this.cbBlendDestination.Location = new System.Drawing.Point(9, 75);
            this.cbBlendDestination.Name = "cbBlendDestination";
            this.cbBlendDestination.Size = new System.Drawing.Size(155, 21);
            this.cbBlendDestination.TabIndex = 27;
            this.cbBlendDestination.SelectedIndexChanged += new System.EventHandler(this.cbBlendDestination_SelectedIndexChanged);
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(6, 19);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(41, 13);
            this.label25.TabIndex = 26;
            this.label25.Text = "Source";
            // 
            // cbBlendSource
            // 
            this.cbBlendSource.FormattingEnabled = true;
            this.cbBlendSource.Items.AddRange(new object[] {
            "Zero",
            "One",
            "SourceColor",
            "InvSourceColor",
            "SourceAlpha",
            "InvSourceAlpha",
            "DestinationAlpha",
            "InvDestinationAlpha",
            "DestinationColor",
            "InvDestinationColor",
            "SourceAlphaSat",
            "BothSourceAlpha",
            "BothInvSourceAlpha",
            "BlendFactor",
            "InvBlendFactor"});
            this.cbBlendSource.Location = new System.Drawing.Point(9, 35);
            this.cbBlendSource.Name = "cbBlendSource";
            this.cbBlendSource.Size = new System.Drawing.Size(155, 21);
            this.cbBlendSource.TabIndex = 23;
            this.cbBlendSource.SelectedIndexChanged += new System.EventHandler(this.cbBlendSource_SelectedIndexChanged);
            // 
            // bRemoveEmitter
            // 
            this.bRemoveEmitter.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bRemoveEmitter.Location = new System.Drawing.Point(161, 259);
            this.bRemoveEmitter.Name = "bRemoveEmitter";
            this.bRemoveEmitter.Size = new System.Drawing.Size(29, 23);
            this.bRemoveEmitter.TabIndex = 25;
            this.bRemoveEmitter.Text = "-";
            this.bRemoveEmitter.UseVisualStyleBackColor = true;
            this.bRemoveEmitter.Click += new System.EventHandler(this.bRemoveEmitter_Click);
            // 
            // cbLooping
            // 
            this.cbLooping.AutoSize = true;
            this.cbLooping.Location = new System.Drawing.Point(16, 312);
            this.cbLooping.Name = "cbLooping";
            this.cbLooping.Size = new System.Drawing.Size(64, 17);
            this.cbLooping.TabIndex = 26;
            this.cbLooping.Text = "Looping";
            this.cbLooping.UseVisualStyleBackColor = true;
            this.cbLooping.CheckedChanged += new System.EventHandler(this.cbLooping_CheckedChanged);
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(133, 296);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(55, 13);
            this.label23.TabIndex = 23;
            this.label23.Text = "Image File";
            // 
            // bLoadTexture
            // 
            this.bLoadTexture.BackColor = System.Drawing.SystemColors.ControlDark;
            this.bLoadTexture.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bLoadTexture.Image = global::Particle_Editor.Properties.Resources.FolderIcon_32x32_;
            this.bLoadTexture.Location = new System.Drawing.Point(136, 312);
            this.bLoadTexture.Name = "bLoadTexture";
            this.bLoadTexture.Size = new System.Drawing.Size(43, 39);
            this.bLoadTexture.TabIndex = 24;
            this.bLoadTexture.Text = "...";
            this.bLoadTexture.UseVisualStyleBackColor = false;
            this.bLoadTexture.Click += new System.EventHandler(this.bLoadTexture_Click);
            // 
            // bAddEmitter
            // 
            this.bAddEmitter.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bAddEmitter.Location = new System.Drawing.Point(128, 259);
            this.bAddEmitter.Name = "bAddEmitter";
            this.bAddEmitter.Size = new System.Drawing.Size(29, 23);
            this.bAddEmitter.TabIndex = 24;
            this.bAddEmitter.Text = "+";
            this.bAddEmitter.UseVisualStyleBackColor = true;
            this.bAddEmitter.Click += new System.EventHandler(this.bAddEmitter_Click);
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(22, 245);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(35, 13);
            this.label22.TabIndex = 23;
            this.label22.Text = "Name";
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(22, 261);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(100, 20);
            this.tbName.TabIndex = 22;
            this.tbName.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tbName_KeyDown);
            // 
            // groupBox8
            // 
            this.groupBox8.Controls.Add(this.nuHeight);
            this.groupBox8.Controls.Add(this.nuWidth);
            this.groupBox8.Controls.Add(this.nuRadius);
            this.groupBox8.Controls.Add(this.labelHeight);
            this.groupBox8.Controls.Add(this.labelWidth);
            this.groupBox8.Controls.Add(this.labelRadius);
            this.groupBox8.Controls.Add(this.label18);
            this.groupBox8.Controls.Add(this.cbShape);
            this.groupBox8.Location = new System.Drawing.Point(6, 629);
            this.groupBox8.Name = "groupBox8";
            this.groupBox8.Size = new System.Drawing.Size(199, 114);
            this.groupBox8.TabIndex = 21;
            this.groupBox8.TabStop = false;
            this.groupBox8.Text = "Shape";
            // 
            // nuHeight
            // 
            this.nuHeight.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuHeight.Location = new System.Drawing.Point(111, 86);
            this.nuHeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nuHeight.Name = "nuHeight";
            this.nuHeight.Size = new System.Drawing.Size(75, 20);
            this.nuHeight.TabIndex = 22;
            this.nuHeight.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuHeight.Visible = false;
            this.nuHeight.ValueChanged += new System.EventHandler(this.nuHeight_ValueChanged);
            // 
            // nuWidth
            // 
            this.nuWidth.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuWidth.Location = new System.Drawing.Point(111, 63);
            this.nuWidth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nuWidth.Name = "nuWidth";
            this.nuWidth.Size = new System.Drawing.Size(75, 20);
            this.nuWidth.TabIndex = 21;
            this.nuWidth.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuWidth.Visible = false;
            this.nuWidth.ValueChanged += new System.EventHandler(this.nuWidth_ValueChanged);
            // 
            // nuRadius
            // 
            this.nuRadius.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuRadius.Location = new System.Drawing.Point(111, 41);
            this.nuRadius.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.nuRadius.Name = "nuRadius";
            this.nuRadius.Size = new System.Drawing.Size(75, 20);
            this.nuRadius.TabIndex = 18;
            this.nuRadius.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuRadius.Visible = false;
            this.nuRadius.ValueChanged += new System.EventHandler(this.nuRadius_ValueChanged);
            // 
            // labelHeight
            // 
            this.labelHeight.AutoSize = true;
            this.labelHeight.Location = new System.Drawing.Point(7, 90);
            this.labelHeight.Name = "labelHeight";
            this.labelHeight.Size = new System.Drawing.Size(38, 13);
            this.labelHeight.TabIndex = 20;
            this.labelHeight.Text = "Height";
            this.labelHeight.Visible = false;
            // 
            // labelWidth
            // 
            this.labelWidth.AutoSize = true;
            this.labelWidth.Location = new System.Drawing.Point(7, 67);
            this.labelWidth.Name = "labelWidth";
            this.labelWidth.Size = new System.Drawing.Size(35, 13);
            this.labelWidth.TabIndex = 19;
            this.labelWidth.Text = "Width";
            this.labelWidth.Visible = false;
            // 
            // labelRadius
            // 
            this.labelRadius.AutoSize = true;
            this.labelRadius.Location = new System.Drawing.Point(6, 45);
            this.labelRadius.Name = "labelRadius";
            this.labelRadius.Size = new System.Drawing.Size(40, 13);
            this.labelRadius.TabIndex = 18;
            this.labelRadius.Text = "Radius";
            this.labelRadius.Visible = false;
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(6, 22);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(31, 13);
            this.label18.TabIndex = 17;
            this.label18.Text = "Type";
            // 
            // cbShape
            // 
            this.cbShape.FormattingEnabled = true;
            this.cbShape.Items.AddRange(new object[] {
            "RECTANGLE",
            "CIRCLE",
            "POINT"});
            this.cbShape.Location = new System.Drawing.Point(95, 14);
            this.cbShape.Name = "cbShape";
            this.cbShape.Size = new System.Drawing.Size(91, 21);
            this.cbShape.TabIndex = 0;
            this.cbShape.SelectedIndexChanged += new System.EventHandler(this.cbShape_SelectedIndexChanged);
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.nuExternalYVelocity);
            this.groupBox7.Controls.Add(this.nuExternalXVelocity);
            this.groupBox7.Controls.Add(this.label16);
            this.groupBox7.Controls.Add(this.label17);
            this.groupBox7.Location = new System.Drawing.Point(9, 443);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(196, 63);
            this.groupBox7.TabIndex = 17;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Force on all Particles";
            // 
            // nuExternalYVelocity
            // 
            this.nuExternalYVelocity.DecimalPlaces = 2;
            this.nuExternalYVelocity.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuExternalYVelocity.Location = new System.Drawing.Point(111, 36);
            this.nuExternalYVelocity.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nuExternalYVelocity.Minimum = new decimal(new int[] {
            10000,
            0,
            0,
            -2147483648});
            this.nuExternalYVelocity.Name = "nuExternalYVelocity";
            this.nuExternalYVelocity.Size = new System.Drawing.Size(75, 20);
            this.nuExternalYVelocity.TabIndex = 16;
            this.nuExternalYVelocity.ValueChanged += new System.EventHandler(this.nuExternalYVelocity_ValueChanged);
            // 
            // nuExternalXVelocity
            // 
            this.nuExternalXVelocity.DecimalPlaces = 2;
            this.nuExternalXVelocity.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuExternalXVelocity.Location = new System.Drawing.Point(110, 14);
            this.nuExternalXVelocity.Maximum = new decimal(new int[] {
            10000,
            0,
            0,
            0});
            this.nuExternalXVelocity.Minimum = new decimal(new int[] {
            10000,
            0,
            0,
            -2147483648});
            this.nuExternalXVelocity.Name = "nuExternalXVelocity";
            this.nuExternalXVelocity.Size = new System.Drawing.Size(75, 20);
            this.nuExternalXVelocity.TabIndex = 15;
            this.nuExternalXVelocity.ValueChanged += new System.EventHandler(this.nuExternalXVelocity_ValueChanged);
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(6, 16);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(54, 13);
            this.label16.TabIndex = 8;
            this.label16.Text = "X Velocity";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(6, 38);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(54, 13);
            this.label17.TabIndex = 9;
            this.label17.Text = "Y Velocity";
            // 
            // nuParticleDuration
            // 
            this.nuParticleDuration.DecimalPlaces = 2;
            this.nuParticleDuration.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuParticleDuration.Location = new System.Drawing.Point(118, 416);
            this.nuParticleDuration.Name = "nuParticleDuration";
            this.nuParticleDuration.Size = new System.Drawing.Size(75, 20);
            this.nuParticleDuration.TabIndex = 20;
            this.nuParticleDuration.Value = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.nuParticleDuration.ValueChanged += new System.EventHandler(this.nuParticleDuration_ValueChanged);
            // 
            // nuNumParticles
            // 
            this.nuNumParticles.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuNumParticles.Location = new System.Drawing.Point(531, 91);
            this.nuNumParticles.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuNumParticles.Name = "nuNumParticles";
            this.nuNumParticles.Size = new System.Drawing.Size(75, 20);
            this.nuNumParticles.TabIndex = 19;
            this.nuNumParticles.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuNumParticles.Visible = false;
            this.nuNumParticles.ValueChanged += new System.EventHandler(this.nuNumParticles_ValueChanged);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(424, 94);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(99, 13);
            this.label15.TabIndex = 18;
            this.label15.Text = "Number of Particles";
            this.label15.Visible = false;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(12, 418);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(85, 13);
            this.label14.TabIndex = 17;
            this.label14.Text = "Particle Duration";
            // 
            // nuEmitterDuration
            // 
            this.nuEmitterDuration.DecimalPlaces = 2;
            this.nuEmitterDuration.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuEmitterDuration.Location = new System.Drawing.Point(119, 390);
            this.nuEmitterDuration.Name = "nuEmitterDuration";
            this.nuEmitterDuration.Size = new System.Drawing.Size(75, 20);
            this.nuEmitterDuration.TabIndex = 15;
            this.nuEmitterDuration.Value = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.nuEmitterDuration.ValueChanged += new System.EventHandler(this.nuEmitterDuration_ValueChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(12, 393);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(82, 13);
            this.label13.TabIndex = 16;
            this.label13.Text = "Emitter Duration";
            // 
            // pTexture
            // 
            this.pTexture.BackColor = System.Drawing.Color.Black;
            this.pTexture.Location = new System.Drawing.Point(272, 662);
            this.pTexture.Name = "pTexture";
            this.pTexture.Size = new System.Drawing.Size(100, 100);
            this.pTexture.TabIndex = 15;
            this.pTexture.Click += new System.EventHandler(this.pTexture_Click);
            this.pTexture.Paint += new System.Windows.Forms.PaintEventHandler(this.pTexture_Paint);
            // 
            // pPreview
            // 
            this.pPreview.BackColor = System.Drawing.Color.White;
            this.pPreview.Location = new System.Drawing.Point(658, 34);
            this.pPreview.Name = "pPreview";
            this.pPreview.Size = new System.Drawing.Size(600, 600);
            this.pPreview.TabIndex = 16;
            this.pPreview.MouseClick += new System.Windows.Forms.MouseEventHandler(this.pPreview_MouseClick);
            this.pPreview.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pPreview_MouseMove);
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(269, 646);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(43, 13);
            this.label24.TabIndex = 25;
            this.label24.Text = "Texture";
            // 
            // bRandom
            // 
            this.bRandom.Location = new System.Drawing.Point(9, 20);
            this.bRandom.Name = "bRandom";
            this.bRandom.Size = new System.Drawing.Size(83, 41);
            this.bRandom.TabIndex = 28;
            this.bRandom.Text = "Randomize";
            this.bRandom.UseVisualStyleBackColor = true;
            this.bRandom.Click += new System.EventHandler(this.bRandom_Click);
            // 
            // cbBurstRand
            // 
            this.cbBurstRand.AutoSize = true;
            this.cbBurstRand.Checked = true;
            this.cbBurstRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbBurstRand.Location = new System.Drawing.Point(13, 41);
            this.cbBurstRand.Name = "cbBurstRand";
            this.cbBurstRand.Size = new System.Drawing.Size(50, 17);
            this.cbBurstRand.TabIndex = 37;
            this.cbBurstRand.Text = "Burst";
            this.cbBurstRand.UseVisualStyleBackColor = true;
            // 
            // cbLoopingRand
            // 
            this.cbLoopingRand.AutoSize = true;
            this.cbLoopingRand.Checked = true;
            this.cbLoopingRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbLoopingRand.Location = new System.Drawing.Point(13, 17);
            this.cbLoopingRand.Name = "cbLoopingRand";
            this.cbLoopingRand.Size = new System.Drawing.Size(64, 17);
            this.cbLoopingRand.TabIndex = 36;
            this.cbLoopingRand.Text = "Looping";
            this.cbLoopingRand.UseVisualStyleBackColor = true;
            // 
            // cbNumParticlesRand
            // 
            this.cbNumParticlesRand.AutoSize = true;
            this.cbNumParticlesRand.Checked = true;
            this.cbNumParticlesRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbNumParticlesRand.Location = new System.Drawing.Point(119, 86);
            this.cbNumParticlesRand.Name = "cbNumParticlesRand";
            this.cbNumParticlesRand.Size = new System.Drawing.Size(115, 17);
            this.cbNumParticlesRand.TabIndex = 38;
            this.cbNumParticlesRand.Text = "Number ofParticles";
            this.cbNumParticlesRand.UseVisualStyleBackColor = true;
            this.cbNumParticlesRand.Visible = false;
            // 
            // cbExternalForceRand
            // 
            this.cbExternalForceRand.AutoSize = true;
            this.cbExternalForceRand.Checked = true;
            this.cbExternalForceRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbExternalForceRand.Location = new System.Drawing.Point(119, 65);
            this.cbExternalForceRand.Name = "cbExternalForceRand";
            this.cbExternalForceRand.Size = new System.Drawing.Size(111, 17);
            this.cbExternalForceRand.TabIndex = 39;
            this.cbExternalForceRand.Text = "Force on Particles";
            this.cbExternalForceRand.UseVisualStyleBackColor = true;
            // 
            // cbEmitterDuration
            // 
            this.cbEmitterDuration.AutoSize = true;
            this.cbEmitterDuration.Checked = true;
            this.cbEmitterDuration.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbEmitterDuration.Location = new System.Drawing.Point(119, 18);
            this.cbEmitterDuration.Name = "cbEmitterDuration";
            this.cbEmitterDuration.Size = new System.Drawing.Size(101, 17);
            this.cbEmitterDuration.TabIndex = 40;
            this.cbEmitterDuration.Text = "Emitter Duration";
            this.cbEmitterDuration.UseVisualStyleBackColor = true;
            // 
            // cbParticleDurationRand
            // 
            this.cbParticleDurationRand.AutoSize = true;
            this.cbParticleDurationRand.Checked = true;
            this.cbParticleDurationRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbParticleDurationRand.Location = new System.Drawing.Point(119, 41);
            this.cbParticleDurationRand.Name = "cbParticleDurationRand";
            this.cbParticleDurationRand.Size = new System.Drawing.Size(104, 17);
            this.cbParticleDurationRand.TabIndex = 41;
            this.cbParticleDurationRand.Text = "Particle Duration";
            this.cbParticleDurationRand.UseVisualStyleBackColor = true;
            // 
            // cbShapeRand
            // 
            this.cbShapeRand.AutoSize = true;
            this.cbShapeRand.Checked = true;
            this.cbShapeRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbShapeRand.Location = new System.Drawing.Point(13, 63);
            this.cbShapeRand.Name = "cbShapeRand";
            this.cbShapeRand.Size = new System.Drawing.Size(57, 17);
            this.cbShapeRand.TabIndex = 42;
            this.cbShapeRand.Text = "Shape";
            this.cbShapeRand.UseVisualStyleBackColor = true;
            // 
            // cbScaleRand
            // 
            this.cbScaleRand.AutoSize = true;
            this.cbScaleRand.Checked = true;
            this.cbScaleRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScaleRand.Location = new System.Drawing.Point(6, 19);
            this.cbScaleRand.Name = "cbScaleRand";
            this.cbScaleRand.Size = new System.Drawing.Size(53, 17);
            this.cbScaleRand.TabIndex = 44;
            this.cbScaleRand.Text = "Scale";
            this.cbScaleRand.UseVisualStyleBackColor = true;
            this.cbScaleRand.CheckedChanged += new System.EventHandler(this.cbScaleRand_CheckedChanged);
            // 
            // cbBlendModeRand
            // 
            this.cbBlendModeRand.AutoSize = true;
            this.cbBlendModeRand.Checked = true;
            this.cbBlendModeRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbBlendModeRand.Location = new System.Drawing.Point(13, 86);
            this.cbBlendModeRand.Name = "cbBlendModeRand";
            this.cbBlendModeRand.Size = new System.Drawing.Size(83, 17);
            this.cbBlendModeRand.TabIndex = 53;
            this.cbBlendModeRand.Text = "Blend Mode";
            this.cbBlendModeRand.UseVisualStyleBackColor = true;
            // 
            // cbScaleCollectionNumRand
            // 
            this.cbScaleCollectionNumRand.AutoSize = true;
            this.cbScaleCollectionNumRand.Checked = true;
            this.cbScaleCollectionNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScaleCollectionNumRand.Location = new System.Drawing.Point(6, 89);
            this.cbScaleCollectionNumRand.Name = "cbScaleCollectionNumRand";
            this.cbScaleCollectionNumRand.Size = new System.Drawing.Size(99, 17);
            this.cbScaleCollectionNumRand.TabIndex = 57;
            this.cbScaleCollectionNumRand.Text = "# of Collections";
            this.cbScaleCollectionNumRand.UseVisualStyleBackColor = true;
            // 
            // cbScaleStopNumRand
            // 
            this.cbScaleStopNumRand.AutoSize = true;
            this.cbScaleStopNumRand.Checked = true;
            this.cbScaleStopNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScaleStopNumRand.Location = new System.Drawing.Point(6, 66);
            this.cbScaleStopNumRand.Name = "cbScaleStopNumRand";
            this.cbScaleStopNumRand.Size = new System.Drawing.Size(75, 17);
            this.cbScaleStopNumRand.TabIndex = 56;
            this.cbScaleStopNumRand.Text = "# of Stops";
            this.cbScaleStopNumRand.UseVisualStyleBackColor = true;
            // 
            // groupBox10
            // 
            this.groupBox10.Controls.Add(this.bCheckAllRand);
            this.groupBox10.Controls.Add(this.bUnCheckAllRand);
            this.groupBox10.Controls.Add(this.groupBox15);
            this.groupBox10.Controls.Add(this.groupBox14);
            this.groupBox10.Controls.Add(this.groupBox13);
            this.groupBox10.Controls.Add(this.groupBox12);
            this.groupBox10.Controls.Add(this.bRandom);
            this.groupBox10.Controls.Add(this.nuNumParticles);
            this.groupBox10.Controls.Add(this.label15);
            this.groupBox10.Location = new System.Drawing.Point(423, 647);
            this.groupBox10.Name = "groupBox10";
            this.groupBox10.Size = new System.Drawing.Size(699, 138);
            this.groupBox10.TabIndex = 58;
            this.groupBox10.TabStop = false;
            // 
            // bCheckAllRand
            // 
            this.bCheckAllRand.Location = new System.Drawing.Point(9, 80);
            this.bCheckAllRand.Name = "bCheckAllRand";
            this.bCheckAllRand.Size = new System.Drawing.Size(83, 23);
            this.bCheckAllRand.TabIndex = 65;
            this.bCheckAllRand.Text = "Check All";
            this.bCheckAllRand.UseVisualStyleBackColor = true;
            this.bCheckAllRand.Click += new System.EventHandler(this.bCheckAllRand_Click);
            // 
            // bUnCheckAllRand
            // 
            this.bUnCheckAllRand.Location = new System.Drawing.Point(9, 106);
            this.bUnCheckAllRand.Name = "bUnCheckAllRand";
            this.bUnCheckAllRand.Size = new System.Drawing.Size(83, 23);
            this.bUnCheckAllRand.TabIndex = 64;
            this.bUnCheckAllRand.Text = "Uncheck All";
            this.bUnCheckAllRand.UseVisualStyleBackColor = true;
            this.bUnCheckAllRand.Click += new System.EventHandler(this.bUnCheckAllRand_Click);
            // 
            // groupBox15
            // 
            this.groupBox15.Controls.Add(this.cbBlendModeRand);
            this.groupBox15.Controls.Add(this.cbShapeRand);
            this.groupBox15.Controls.Add(this.cbParticleDurationRand);
            this.groupBox15.Controls.Add(this.cbNumParticlesRand);
            this.groupBox15.Controls.Add(this.cbEmitterDuration);
            this.groupBox15.Controls.Add(this.cbExternalForceRand);
            this.groupBox15.Controls.Add(this.cbBurstRand);
            this.groupBox15.Controls.Add(this.cbLoopingRand);
            this.groupBox15.Location = new System.Drawing.Point(98, 16);
            this.groupBox15.Name = "groupBox15";
            this.groupBox15.Size = new System.Drawing.Size(252, 113);
            this.groupBox15.TabIndex = 63;
            this.groupBox15.TabStop = false;
            this.groupBox15.Text = "Emitter";
            // 
            // groupBox14
            // 
            this.groupBox14.Controls.Add(this.cbVelocityCollectionNumRand);
            this.groupBox14.Controls.Add(this.cbVelocityStopNumRand);
            this.groupBox14.Controls.Add(this.cbVelocityCoefRand);
            this.groupBox14.Controls.Add(this.cbVelocityRand);
            this.groupBox14.Location = new System.Drawing.Point(582, 15);
            this.groupBox14.Name = "groupBox14";
            this.groupBox14.Size = new System.Drawing.Size(107, 60);
            this.groupBox14.TabIndex = 62;
            this.groupBox14.TabStop = false;
            this.groupBox14.Text = "Velocity";
            // 
            // cbVelocityCollectionNumRand
            // 
            this.cbVelocityCollectionNumRand.AutoSize = true;
            this.cbVelocityCollectionNumRand.Checked = true;
            this.cbVelocityCollectionNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbVelocityCollectionNumRand.Location = new System.Drawing.Point(6, 89);
            this.cbVelocityCollectionNumRand.Name = "cbVelocityCollectionNumRand";
            this.cbVelocityCollectionNumRand.Size = new System.Drawing.Size(99, 17);
            this.cbVelocityCollectionNumRand.TabIndex = 57;
            this.cbVelocityCollectionNumRand.Text = "# of Collections";
            this.cbVelocityCollectionNumRand.UseVisualStyleBackColor = true;
            // 
            // cbVelocityStopNumRand
            // 
            this.cbVelocityStopNumRand.AutoSize = true;
            this.cbVelocityStopNumRand.Checked = true;
            this.cbVelocityStopNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbVelocityStopNumRand.Location = new System.Drawing.Point(6, 66);
            this.cbVelocityStopNumRand.Name = "cbVelocityStopNumRand";
            this.cbVelocityStopNumRand.Size = new System.Drawing.Size(75, 17);
            this.cbVelocityStopNumRand.TabIndex = 56;
            this.cbVelocityStopNumRand.Text = "# of Stops";
            this.cbVelocityStopNumRand.UseVisualStyleBackColor = true;
            // 
            // cbVelocityCoefRand
            // 
            this.cbVelocityCoefRand.AutoSize = true;
            this.cbVelocityCoefRand.Checked = true;
            this.cbVelocityCoefRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbVelocityCoefRand.Location = new System.Drawing.Point(6, 40);
            this.cbVelocityCoefRand.Name = "cbVelocityCoefRand";
            this.cbVelocityCoefRand.Size = new System.Drawing.Size(84, 17);
            this.cbVelocityCoefRand.TabIndex = 59;
            this.cbVelocityCoefRand.Text = "Time on Life";
            this.cbVelocityCoefRand.UseVisualStyleBackColor = true;
            // 
            // cbVelocityRand
            // 
            this.cbVelocityRand.AutoSize = true;
            this.cbVelocityRand.Checked = true;
            this.cbVelocityRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbVelocityRand.Location = new System.Drawing.Point(6, 19);
            this.cbVelocityRand.Name = "cbVelocityRand";
            this.cbVelocityRand.Size = new System.Drawing.Size(63, 17);
            this.cbVelocityRand.TabIndex = 44;
            this.cbVelocityRand.Text = "Velocity";
            this.cbVelocityRand.UseVisualStyleBackColor = true;
            this.cbVelocityRand.CheckedChanged += new System.EventHandler(this.cbVelocityRand_CheckedChanged);
            // 
            // groupBox13
            // 
            this.groupBox13.Controls.Add(this.cbColorCollectionNumRand);
            this.groupBox13.Controls.Add(this.cbColorStopNumRand);
            this.groupBox13.Controls.Add(this.cbColorCoefRand);
            this.groupBox13.Controls.Add(this.cbColorRand);
            this.groupBox13.Location = new System.Drawing.Point(356, 16);
            this.groupBox13.Name = "groupBox13";
            this.groupBox13.Size = new System.Drawing.Size(107, 59);
            this.groupBox13.TabIndex = 62;
            this.groupBox13.TabStop = false;
            this.groupBox13.Text = "Color";
            // 
            // cbColorCollectionNumRand
            // 
            this.cbColorCollectionNumRand.AutoSize = true;
            this.cbColorCollectionNumRand.Checked = true;
            this.cbColorCollectionNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbColorCollectionNumRand.Location = new System.Drawing.Point(6, 89);
            this.cbColorCollectionNumRand.Name = "cbColorCollectionNumRand";
            this.cbColorCollectionNumRand.Size = new System.Drawing.Size(99, 17);
            this.cbColorCollectionNumRand.TabIndex = 57;
            this.cbColorCollectionNumRand.Text = "# of Collections";
            this.cbColorCollectionNumRand.UseVisualStyleBackColor = true;
            // 
            // cbColorStopNumRand
            // 
            this.cbColorStopNumRand.AutoSize = true;
            this.cbColorStopNumRand.Checked = true;
            this.cbColorStopNumRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbColorStopNumRand.Location = new System.Drawing.Point(6, 66);
            this.cbColorStopNumRand.Name = "cbColorStopNumRand";
            this.cbColorStopNumRand.Size = new System.Drawing.Size(75, 17);
            this.cbColorStopNumRand.TabIndex = 56;
            this.cbColorStopNumRand.Text = "# of Stops";
            this.cbColorStopNumRand.UseVisualStyleBackColor = true;
            // 
            // cbColorCoefRand
            // 
            this.cbColorCoefRand.AutoSize = true;
            this.cbColorCoefRand.Checked = true;
            this.cbColorCoefRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbColorCoefRand.Location = new System.Drawing.Point(6, 40);
            this.cbColorCoefRand.Name = "cbColorCoefRand";
            this.cbColorCoefRand.Size = new System.Drawing.Size(84, 17);
            this.cbColorCoefRand.TabIndex = 59;
            this.cbColorCoefRand.Text = "Time on Life";
            this.cbColorCoefRand.UseVisualStyleBackColor = true;
            // 
            // cbColorRand
            // 
            this.cbColorRand.AutoSize = true;
            this.cbColorRand.Checked = true;
            this.cbColorRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbColorRand.Location = new System.Drawing.Point(6, 19);
            this.cbColorRand.Name = "cbColorRand";
            this.cbColorRand.Size = new System.Drawing.Size(50, 17);
            this.cbColorRand.TabIndex = 44;
            this.cbColorRand.Text = "Color";
            this.cbColorRand.UseVisualStyleBackColor = true;
            this.cbColorRand.CheckedChanged += new System.EventHandler(this.cbColorRand_CheckedChanged);
            // 
            // groupBox12
            // 
            this.groupBox12.Controls.Add(this.cbScaleCollectionNumRand);
            this.groupBox12.Controls.Add(this.cbScaleStopNumRand);
            this.groupBox12.Controls.Add(this.cbScaleCoefRand);
            this.groupBox12.Controls.Add(this.cbScaleRand);
            this.groupBox12.Location = new System.Drawing.Point(469, 15);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(107, 60);
            this.groupBox12.TabIndex = 61;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Size";
            // 
            // cbScaleCoefRand
            // 
            this.cbScaleCoefRand.AutoSize = true;
            this.cbScaleCoefRand.Checked = true;
            this.cbScaleCoefRand.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbScaleCoefRand.Location = new System.Drawing.Point(6, 41);
            this.cbScaleCoefRand.Name = "cbScaleCoefRand";
            this.cbScaleCoefRand.Size = new System.Drawing.Size(84, 17);
            this.cbScaleCoefRand.TabIndex = 59;
            this.cbScaleCoefRand.Text = "Time on Life";
            this.cbScaleCoefRand.UseVisualStyleBackColor = true;
            // 
            // groupBox11
            // 
            this.groupBox11.Controls.Add(this.cbAnchorToBG);
            this.groupBox11.Controls.Add(this.bLoadBG);
            this.groupBox11.Location = new System.Drawing.Point(1157, 634);
            this.groupBox11.Name = "groupBox11";
            this.groupBox11.Size = new System.Drawing.Size(101, 124);
            this.groupBox11.TabIndex = 59;
            this.groupBox11.TabStop = false;
            this.groupBox11.Text = "Preview Options";
            this.groupBox11.Visible = false;
            // 
            // cbAnchorToBG
            // 
            this.cbAnchorToBG.AutoSize = true;
            this.cbAnchorToBG.Location = new System.Drawing.Point(6, 85);
            this.cbAnchorToBG.Name = "cbAnchorToBG";
            this.cbAnchorToBG.Size = new System.Drawing.Size(90, 17);
            this.cbAnchorToBG.TabIndex = 58;
            this.cbAnchorToBG.Text = "Anchor to BG";
            this.cbAnchorToBG.UseVisualStyleBackColor = true;
            // 
            // bLoadBG
            // 
            this.bLoadBG.Location = new System.Drawing.Point(6, 19);
            this.bLoadBG.Name = "bLoadBG";
            this.bLoadBG.Size = new System.Drawing.Size(89, 49);
            this.bLoadBG.TabIndex = 0;
            this.bLoadBG.Text = "Load Background Image";
            this.bLoadBG.UseVisualStyleBackColor = true;
            this.bLoadBG.Click += new System.EventHandler(this.bLoadBG_Click);
            // 
            // checkBox9
            // 
            this.checkBox9.AutoSize = true;
            this.checkBox9.Location = new System.Drawing.Point(272, 768);
            this.checkBox9.Name = "checkBox9";
            this.checkBox9.Size = new System.Drawing.Size(117, 17);
            this.checkBox9.TabIndex = 60;
            this.checkBox9.Text = "Use For All Emitters";
            this.checkBox9.UseVisualStyleBackColor = true;
            this.checkBox9.Visible = false;
            this.checkBox9.CheckedChanged += new System.EventHandler(this.checkBox9_CheckedChanged);
            // 
            // nuScale
            // 
            this.nuScale.DecimalPlaces = 2;
            this.nuScale.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuScale.Location = new System.Drawing.Point(120, 365);
            this.nuScale.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.nuScale.Name = "nuScale";
            this.nuScale.Size = new System.Drawing.Size(75, 20);
            this.nuScale.TabIndex = 67;
            this.nuScale.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nuScale.ValueChanged += new System.EventHandler(this.nuScale_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 368);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(72, 13);
            this.label2.TabIndex = 66;
            this.label2.Text = "Particle Scale";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ControlLightLight;
            this.ClientSize = new System.Drawing.Size(1279, 799);
            this.Controls.Add(this.checkBox9);
            this.Controls.Add(this.groupBox11);
            this.Controls.Add(this.groupBox10);
            this.Controls.Add(this.label24);
            this.Controls.Add(this.pPreview);
            this.Controls.Add(this.pTexture);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.gbVelStop.ResumeLayout(false);
            this.gbVelStop.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVelocityTime)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuYVelocity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuXVelocity)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.gbSizeStop.ResumeLayout(false);
            this.gbSizeStop.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuSizeWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuSizeHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSizeTime)).EndInit();
            this.gbColorStop.ResumeLayout(false);
            this.gbColorStop.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarColorTime)).EndInit();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox9.ResumeLayout(false);
            this.groupBox9.PerformLayout();
            this.groupBox8.ResumeLayout(false);
            this.groupBox8.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuRadius)).EndInit();
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuExternalYVelocity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuExternalXVelocity)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuParticleDuration)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuNumParticles)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nuEmitterDuration)).EndInit();
            this.groupBox10.ResumeLayout(false);
            this.groupBox10.PerformLayout();
            this.groupBox15.ResumeLayout(false);
            this.groupBox15.PerformLayout();
            this.groupBox14.ResumeLayout(false);
            this.groupBox14.PerformLayout();
            this.groupBox13.ResumeLayout(false);
            this.groupBox13.PerformLayout();
            this.groupBox12.ResumeLayout(false);
            this.groupBox12.PerformLayout();
            this.groupBox11.ResumeLayout(false);
            this.groupBox11.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nuScale)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ListBox lbEmitters;
        private System.Windows.Forms.ListBox lbParticles;
        private System.Windows.Forms.ListBox lbColors;
        private System.Windows.Forms.ListBox lbSizes;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lParticleList;
        private System.Windows.Forms.Label lNote;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label lColorCollection;
        private System.Windows.Forms.GroupBox gbVelStop;
        private System.Windows.Forms.Label lSizeCollection;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox gbColorStop;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button bChangeColor;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.NumericUpDown nuYVelocity;
        private System.Windows.Forms.NumericUpDown nuXVelocity;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.GroupBox gbSizeStop;
        private System.Windows.Forms.NumericUpDown nuSizeWidth;
        private System.Windows.Forms.NumericUpDown nuSizeHeight;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.NumericUpDown nuExternalYVelocity;
        private System.Windows.Forms.NumericUpDown nuExternalXVelocity;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.NumericUpDown nuParticleDuration;
        private System.Windows.Forms.NumericUpDown nuNumParticles;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.NumericUpDown nuEmitterDuration;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.GroupBox groupBox8;
        private System.Windows.Forms.ComboBox cbShape;
        private System.Windows.Forms.Panel pTexture;
        private System.Windows.Forms.Panel pPreview;
        private System.Windows.Forms.Button bRemoveEmitter;
        private System.Windows.Forms.Button bAddEmitter;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.NumericUpDown nuHeight;
        private System.Windows.Forms.NumericUpDown nuWidth;
        private System.Windows.Forms.NumericUpDown nuRadius;
        private System.Windows.Forms.Label labelHeight;
        private System.Windows.Forms.Label labelWidth;
        private System.Windows.Forms.Label labelRadius;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.CheckBox cbBurst;
        private System.Windows.Forms.CheckBox cbLooping;
        private System.Windows.Forms.TextBox tbFilepath;
        private System.Windows.Forms.ComboBox cbBlendSource;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Button bLoadTexture;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.Button bSelectAllParticles;
        private System.Windows.Forms.Button bRemoveVelocity;
        private System.Windows.Forms.Button bAddVelocity;
        private System.Windows.Forms.Label lVelocityCollection;
        private System.Windows.Forms.ListBox lbVelocities;
        private System.Windows.Forms.Button bRemoveColor;
        private System.Windows.Forms.Button bAddColor;
        private System.Windows.Forms.Button bRemoveSize;
        private System.Windows.Forms.Button bAddSize;
        private System.Windows.Forms.Button bRandom;
        private System.Windows.Forms.GroupBox groupBox9;
        private System.Windows.Forms.Label label29;
        private System.Windows.Forms.ComboBox cbBlendDestination;
        private System.Windows.Forms.ToolStripMenuItem openXMLToolStripMenuItem;
        private System.Windows.Forms.Button bRemoveParticle;
        private System.Windows.Forms.Button bAddParticle;
        private System.Windows.Forms.ToolStripMenuItem randomizeCurrentEmitterToolStripMenuItem;
        private System.Windows.Forms.Label label32;
        private System.Windows.Forms.Label label33;
        private System.Windows.Forms.TrackBar trackBarColorTime;
        private System.Windows.Forms.Label label27;
        private System.Windows.Forms.Label label30;
        private System.Windows.Forms.TrackBar trackBarVelocityTime;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TrackBar trackBarSizeTime;
        private System.Windows.Forms.ToolTip toolTip1;
        private System.Windows.Forms.CheckBox cbBurstRand;
        private System.Windows.Forms.CheckBox cbLoopingRand;
        private System.Windows.Forms.CheckBox cbNumParticlesRand;
        private System.Windows.Forms.CheckBox cbExternalForceRand;
        private System.Windows.Forms.CheckBox cbEmitterDuration;
        private System.Windows.Forms.CheckBox cbParticleDurationRand;
        private System.Windows.Forms.CheckBox cbShapeRand;
        private System.Windows.Forms.CheckBox cbScaleRand;
        private System.Windows.Forms.CheckBox cbBlendModeRand;
        private System.Windows.Forms.CheckBox cbScaleCollectionNumRand;
        private System.Windows.Forms.CheckBox cbScaleStopNumRand;
        private System.Windows.Forms.GroupBox groupBox10;
        private System.Windows.Forms.GroupBox groupBox11;
        private System.Windows.Forms.CheckBox cbAnchorToBG;
        private System.Windows.Forms.Button bLoadBG;
        private System.Windows.Forms.GroupBox groupBox12;
        private System.Windows.Forms.CheckBox cbScaleCoefRand;
        private System.Windows.Forms.GroupBox groupBox15;
        private System.Windows.Forms.GroupBox groupBox14;
        private System.Windows.Forms.CheckBox cbVelocityCollectionNumRand;
        private System.Windows.Forms.CheckBox cbVelocityStopNumRand;
        private System.Windows.Forms.CheckBox cbVelocityCoefRand;
        private System.Windows.Forms.CheckBox cbVelocityRand;
        private System.Windows.Forms.GroupBox groupBox13;
        private System.Windows.Forms.CheckBox cbColorCollectionNumRand;
        private System.Windows.Forms.CheckBox cbColorStopNumRand;
        private System.Windows.Forms.CheckBox cbColorCoefRand;
        private System.Windows.Forms.CheckBox cbColorRand;
        private System.Windows.Forms.CheckBox checkBox9;
        private System.Windows.Forms.Button bAddTenParticles;
        private System.Windows.Forms.ToolStripMenuItem openXMLAndAppendToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.Button bCheckAllRand;
        private System.Windows.Forms.Button bUnCheckAllRand;
        private System.Windows.Forms.NumericUpDown nuScale;
        private System.Windows.Forms.Label label2;
    }
}

