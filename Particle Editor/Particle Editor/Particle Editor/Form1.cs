using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.IO;
using System.Xml;
using System.Xml.Linq;
using SGP;
using System.Runtime.InteropServices;

using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;

namespace Particle_Editor
{
    public partial class Form1 : Form
    {
        public bool Looping = true; 

        ManagedDirect3D d3d = ManagedDirect3D.Instance;

        List<particleEmitter> m_lEmitters;
        int selected = -1;
        int m_nID = -1;
        DateTime gameTime = new DateTime();
        string currentPath;
        string currentFilename;
        float duration = 0.0f;
        //long lGameTime = 0;
        Random rand;
        RandomizeBools bRand = new RandomizeBools();
        bool bSingleImage = false;
        string imageLocation;
        string debugCoef;
        int particleSelected;
        int nBG_ID = -1;
        Vector2 vBGpos = new Vector2(0, 0);

        [DllImport("kernel32.dll")]
        public static extern long GetTickCount();

        List<indexInTo> colorBox = new List<indexInTo>();
        List<indexInTo> sizeBox = new List<indexInTo>();
        List<indexInTo> velBox = new List<indexInTo>();

        public Form1()
        {
            InitializeComponent();

            ManagedDirect3D.Instance.InitManagedDirect3D(pPreview);
            ManagedTextureManager.Instance.InitManagedTextureManager(
                ManagedDirect3D.Instance.Device,
                ManagedDirect3D.Instance.Sprite);

            this.DoubleBuffered = true;

            m_lEmitters = new List<particleEmitter>();
            m_nID = -1;

            gameTime = System.DateTime.Now;

            currentPath = Directory.GetCurrentDirectory();
            rand = new Random(System.DateTime.Now.Millisecond);

            addEmitter("Default Emitter");

            hideParticleControls();

            bSingleImage = checkBox9.Checked;
            //lGameTime = GetTickCount();
        }

        public void Update()
        {
            DateTime now = (System.DateTime.Now);
            TimeSpan fElapsed = (now - gameTime);
            gameTime = now;
            duration += (float)fElapsed.TotalSeconds;
            
            //long now = GetTickCount();
            //float fElapsed = (now - lGameTime) / 1000.0f;
            //lGameTime = now;

            //tbFilepath.Text = duration.ToString();
            //tbFilepath.Text = Convert.ToString(trackBarColorTime.Value / 100.0f);
            if( debugCoef != null )
                tbFilepath.Text = debugCoef;

            

            if (m_lEmitters.Count > 0 && selected < m_lEmitters.Count && selected >= 0)
                m_lEmitters[selected].UpdateParticles((float)fElapsed.TotalSeconds);
        }

        
        class RandomizeBools
        {
            class RondomizeEmitterBools
            {
                bool looping = true;
                bool burst = true;
                bool numParts = true;
                bool blendMode = true;
                bool emitterDuration = true;
                bool particleDuration = true;
                bool force = true;
                bool shape = true;
            }

            class RandomizeColorBools
            {
                bool color = true;
                bool coef = true;
                bool stops = true;
                bool collections = true;
            }

            class RandomizeSizeBools
            {
                bool x = true;
                bool y = true;
                bool coef = true;
                bool stops = true;
                bool collections = true;
            }

            class RandomizeVelocityBools
            {
                bool velocity = true;
                bool coef = true;
                bool stops = true;
                bool collections = true;
            }

            RondomizeEmitterBools bEmitter;
            RandomizeColorBools bColor;
            RandomizeSizeBools bSize;
            RandomizeVelocityBools bVel;
        }

        public void Render()
        {
            ManagedDirect3D.Instance.DeviceBegin();
            ManagedDirect3D.Instance.SpriteBegin();

            ManagedDirect3D.Instance.Clear(Color.Black);

            if (m_lEmitters.Count > 0 && selected < m_lEmitters.Count && selected >= 0)            
            {
                if (m_lEmitters[selected].imageID >= 0 || (bSingleImage && m_nID >= 0))
                {
                    
                    if (m_lEmitters[selected].destBlendMode >= (int)Microsoft.DirectX.Direct3D.Blend.Zero &&
                        m_lEmitters[selected].destBlendMode <= (int)Microsoft.DirectX.Direct3D.Blend.InvBlendFactor)
                    {
                        d3d.Device.SetRenderState(RenderStates.DestinationBlend, m_lEmitters[selected].destBlendMode);
                    }

                    if (m_lEmitters[selected].sourceBlendMode >= (int)Microsoft.DirectX.Direct3D.Blend.Zero &&
                        m_lEmitters[selected].sourceBlendMode <= (int)Microsoft.DirectX.Direct3D.Blend.InvBlendFactor)
                    {
                        d3d.Device.SetRenderState(RenderStates.SourceBlend, m_lEmitters[selected].sourceBlendMode);
                    }
                    
                    m_lEmitters[selected].RenderParticles(bSingleImage ? m_nID : -2);
                }
            }

            if (nBG_ID != -1)
            {
                //d3d.Device.SetRenderState(RenderStates.DestinationBlend, 14); // default Destination Color mode
                //d3d.Device.SetRenderState(RenderStates.SourceBlend, 5); // default Source Color mode
                ManagedTextureManager mTM = ManagedTextureManager.Instance;
                Rectangle r = new Rectangle(new Point((int)vBGpos.X, (int)vBGpos.Y),
                    new Size(mTM.GetTextureWidth(nBG_ID), mTM.GetTextureHeight(nBG_ID)));
                mTM.Draw(nBG_ID, (int)vBGpos.X, (int)vBGpos.Y, 1.0f, 1.0f, r, 0, 0, 0.0f, Color.White);
            }

            ManagedDirect3D.Instance.SpriteEnd();
            ManagedDirect3D.Instance.DeviceEnd();

            ManagedDirect3D.Instance.Present();
        }

        private bool emitterIsSelected()
        {
            if (selected < m_lEmitters.Count() && selected >= 0)
                return true;
            else
                return false;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Looping = false;
            ManagedDirect3D.Instance.Shutdown();
        }

        private void showParticleControls()
        {
            lParticleList.Visible = true;
            lbParticles.Visible = true;
            bAddParticle.Visible = true;
            bAddTenParticles.Visible = true;
            bRemoveParticle.Visible = true;
            bSelectAllParticles.Visible = true;
        }

        private void changeSelectedEmitter(int index)
        {
            if (selected != lbEmitters.SelectedIndex)
                hideParticleControls();

            if (index >= 0 && index < m_lEmitters.Count())
            {
                if (tbName.Text != null)
                    tbName.Text = m_lEmitters[index].name;
                else
                    tbName.Text = "";
                showParticleControls();

                selected = index;

                cbLooping.Checked = m_lEmitters[selected].isLooping;
                cbBurst.Checked = m_lEmitters[selected].isBurst;
                nuEmitterDuration.Value = (decimal)m_lEmitters[selected].eDuration;
                nuParticleDuration.Value = (decimal)m_lEmitters[selected].pDuration;
                nuExternalXVelocity.Value = (decimal)m_lEmitters[selected].force.X;
                nuExternalYVelocity.Value = (decimal)m_lEmitters[selected].force.Y;
                nuScale.Value = (decimal)m_lEmitters[selected].particleScalar;

                // do this first so it doesn't mess up setting the 
                // child controls (width, height or radius)
                int shape = cbShape.SelectedIndex = m_lEmitters[index].shape;
                if (shape == 0 && m_lEmitters[index].pRect.Width > 1
                    && m_lEmitters[index].pRect.Height > 1)
                {
                    nuWidth.Value = m_lEmitters[index].pRect.Width;
                    nuHeight.Value = m_lEmitters[index].pRect.Height;
                }
                if (shape == 1 && m_lEmitters[index].radius > 1)
                {
                    nuRadius.Value = (int)m_lEmitters[index].radius;
                }

                cbBlendSource.SelectedIndex = m_lEmitters[index].sourceBlendMode-1;
                cbBlendDestination.SelectedIndex = m_lEmitters[index].destBlendMode - 1;

                //string path = currentPath + "\\" + m_lEmitters[index].filename;
                string path = m_lEmitters[index].filepath + m_lEmitters[index].filename;

                if (File.Exists(path) && ! bSingleImage)
                {
                    m_nID = ManagedTextureManager.Instance.LoadTexture(path, Color.FromArgb(255, 255, 255, 255).ToArgb());
                    pTexture.Invalidate();
                }
                else if( ! bSingleImage )
                {
                    string message = "No Image Found. " + m_lEmitters[index].filename + ".\nLoad Image?";
                    string caption = "Load Image";
                    var result = MessageBox.Show(message, caption,
                                   MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                    if (result == DialogResult.Yes)
                    {
                        LoadImage(index);
                        pTexture.Invalidate();
                    }
                }

                if(m_nID != -1 )
                    pTexture.Invalidate();

                int i = 0;
                lbParticles.Items.Clear();
                foreach(tParticle p in m_lEmitters[selected].particles)
                {
                    lbParticles.Items.Add(i);
                    ++i;
                }

                //lbParticles.SelectedIndex = 0;
                //particleChange();
                
                

                m_lEmitters[selected].pos.X = pPreview.Width / 2.0f;
                m_lEmitters[selected].pos.Y = pPreview.Height / 2.0f;

                m_lEmitters[selected].startPos = m_lEmitters[selected].pos;
                m_lEmitters[selected].reset();
            }
        }

        class indexInTo
        {
            public int particle = -1;
            public int collection = -1;
            public int indexInCollectionIndices = -1;
        }

        

        public void particleChange()
        {
            
            lbColors.Items.Clear();
            lbSizes.Items.Clear();
            lbVelocities.Items.Clear();

            gbColorStop.Visible = false;
            gbSizeStop.Visible = false;
            gbVelStop.Visible = false;

            ////////////////////////
            // Color
            ////////////////////////
            List<indexInTo> masterList = new List<indexInTo>();
            
            // Populate masterList with ALL colorStops in uColors
            for ( int i = 0; i < m_lEmitters[selected].particles.Count(); ++i )
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                int x = 0;
                foreach( int ColorIndex in m_lEmitters[selected].particles[i].colorIndices )
                {
                    indexInTo cint = new indexInTo();
                    cint.indexInCollectionIndices = x;
                    cint.collection = ColorIndex;
                    cint.particle = i;
                    masterList.Add(cint);
                    ++x;
                }
            }

            // Rmove any unique stops that do not match with the currently selected particles
            for ( int i = 0; i < m_lEmitters[selected].particles.Count(); ++i )
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                for( int m = 0; m < masterList.Count(); ++m)
                {
                    if( false == m_lEmitters[selected].particles[i].colorIndices.Contains(masterList[m].collection) )
                    {
                        indexInTo removeMe = masterList[m];
                        // Rmove Duplicates
                        while( masterList.Remove(removeMe) ) /*Do Nothing*/ ;
                    }
                }
            }
            
            // Add the color colection index to the lisbox
            for (int i = 0; i < masterList.Count(); ++i)
            {
                if( ! lbColors.Items.Contains(masterList[i].collection) )
                    lbColors.Items.Add(masterList[i].collection);
            }


            ////////////////////////
            // Size
            ////////////////////////
            masterList = new List<indexInTo>();

            // Populate masterList with ALL sizeStops in uSizes
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                int x = 0;
                foreach (int SizeIndex in m_lEmitters[selected].particles[i].sizeIndices)
                {
                    indexInTo cint = new indexInTo();
                    cint.indexInCollectionIndices = x;
                    cint.collection = SizeIndex;
                    cint.particle = i;
                    masterList.Add(cint);
                    ++x;
                }
            }

            // Rmove any unique stops that do not match with the currently selected particles
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                for (int m = 0; m < masterList.Count(); ++m)
                {
                    if (false == m_lEmitters[selected].particles[i].sizeIndices.Contains(masterList[m].collection))
                    {
                        indexInTo removeMe = masterList[m];
                        // Rmove Duplicates
                        while (masterList.Remove(removeMe)) /*Do Nothing*/ ;
                    }
                }
            }

            // Add the color colection index to the lisbox
            for (int i = 0; i < masterList.Count(); ++i)
            {

                if ( ! lbSizes.Items.Contains(masterList[i].collection))
                    lbSizes.Items.Add(masterList[i].collection);
            }

            ////////////////////////
            // Velocity
            ////////////////////////
            masterList = new List<indexInTo>();

            // Populate masterList with ALL sizeStops in uSizes
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                int x = 0;
                foreach (int VelIndex in m_lEmitters[selected].particles[i].velIndices)
                {
                    indexInTo cint = new indexInTo();
                    cint.indexInCollectionIndices = x;
                    cint.collection = VelIndex;
                    cint.particle = i;
                    masterList.Add(cint);
                    ++x;
                }
            }

            // Rmove any unique stops that do not match with the currently selected particles
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                for (int m = 0; m < masterList.Count(); ++m)
                {
                    if (false == m_lEmitters[selected].particles[i].velIndices.Contains(masterList[m].collection))
                    {
                        indexInTo removeMe = masterList[m];
                        // Rmove Duplicates
                        while (masterList.Remove(removeMe)) /*Do Nothing*/ ;
                    }
                }
            }

            // Add the color colection index to the lisbox
            for (int i = 0; i < masterList.Count(); ++i)
            {
                if (!lbVelocities.Items.Contains(masterList[i].collection))
                    lbVelocities.Items.Add(masterList[i].collection);
            }

            showCollectionControls();
        }

        private void showCollectionControls()
        {
            // Color
            lColorCollection.Visible = true;
            lbColors.Visible = true;
            bAddColor.Visible = true;
            bRemoveColor.Visible = true;
            
            // Size
            lSizeCollection.Visible = true;
            lbSizes.Visible = true;
            bAddSize.Visible = true;
            bRemoveSize.Visible = true;

            // Velocity
            lVelocityCollection.Visible = true;
            lbVelocities.Visible = true;
            bAddVelocity.Visible = true;
            bRemoveVelocity.Visible = true;

        }

       

        private void trackBarColorTime_MouseUp(object sender, MouseEventArgs e)
        {
            ChangeColorTime();
        }

        public void ChangeColorTime()
        {
            int pind = -1;
            int cind = -1;
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for( int a = 0; a < m_lEmitters[selected].particles[i].colorIndices.Count(); ++a)
                {
                    if ((int)lbColors.SelectedItem != m_lEmitters[selected].particles[i].colorIndices[a]) continue;
                    m_lEmitters[selected].uColors[m_lEmitters[selected].particles[i].colorIndices[a]].coef =
                        trackBarColorTime.Value / 100.0f;
                    pind = i;
                    cind = a;
                }

            }

            debugCoef = m_lEmitters[selected].uColors[Convert.ToInt32(lbColors.SelectedItem.ToString())].coef.ToString();
            int foo = 0;
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for( int a = 0; a < m_lEmitters[selected].particles[i].colorIndices.Count(); ++a)
                {
                    for (int b = 0; b < m_lEmitters[selected].particles[i].colorIndices.Count(); ++b)
                    {
                        if (m_lEmitters[selected].uColors[m_lEmitters[selected].particles[i].colorIndices[a]].coef <
                            m_lEmitters[selected].uColors[m_lEmitters[selected].particles[i].colorIndices[b]].coef)
                        {
                            int swapt = m_lEmitters[selected].particles[i].colorIndices[a];
                            m_lEmitters[selected].particles[i].colorIndices[a] = m_lEmitters[selected].particles[i].colorIndices[b];
                            m_lEmitters[selected].particles[i].colorIndices[b] = swapt;
                            if( pind == i && (cind == a || cind == b) )
                            {
                                ++foo;
                            }
                        }
                    }
                }

            }

        }

        private void trackBarSizeTime_MouseUp(object sender, MouseEventArgs e)
        {
            ChangeSizeTime();
        }

        private void ChangeSizeTime()
        {
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for (int a = 0; a < m_lEmitters[selected].particles[i].sizeIndices.Count(); ++a)
                {
                    if ((int)lbSizes.SelectedItem != m_lEmitters[selected].particles[i].sizeIndices[a]) continue;
                    m_lEmitters[selected].uSizes[m_lEmitters[selected].particles[i].sizeIndices[a]].coef =
                         trackBarSizeTime.Value / 100.0f; 
                }

            }


            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for (int a = 0; a < m_lEmitters[selected].particles[i].sizeIndices.Count(); ++a)
                {
                    for (int b = 0; b < m_lEmitters[selected].particles[i].sizeIndices.Count(); ++b)
                    {
                        if (m_lEmitters[selected].uSizes[m_lEmitters[selected].particles[i].sizeIndices[a]].coef <
                            m_lEmitters[selected].uSizes[m_lEmitters[selected].particles[i].sizeIndices[b]].coef)
                        {
                            int swapt = m_lEmitters[selected].particles[i].sizeIndices[a];
                            m_lEmitters[selected].particles[i].sizeIndices[a] = m_lEmitters[selected].particles[i].sizeIndices[b];
                            m_lEmitters[selected].particles[i].sizeIndices[b] = swapt;

                        }
                    }
                }
            }

        }
        
        private void trackBarVelocityTime_MouseUp(object sender, MouseEventArgs e)
        {
            ChangeVelocityTime();
        }

        private void ChangeVelocityTime()
        {
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for (int a = 0; a < m_lEmitters[selected].particles[i].velIndices.Count(); ++a)
                {
                    if ((int)lbVelocities.SelectedItem != m_lEmitters[selected].particles[i].velIndices[a]) continue;
                    m_lEmitters[selected].uVels[m_lEmitters[selected].particles[i].velIndices[a]].coef =
                        trackBarVelocityTime.Value / 100.0f;
                }

            }


            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;


                for (int a = 0; a < m_lEmitters[selected].particles[i].velIndices.Count(); ++a)
                {
                    for (int b = 0; b < m_lEmitters[selected].particles[i].velIndices.Count(); ++b)
                    {
                        if (m_lEmitters[selected].uVels[m_lEmitters[selected].particles[i].velIndices[a]].coef <
                            m_lEmitters[selected].uVels[m_lEmitters[selected].particles[i].velIndices[b]].coef)
                        {
                            int swapt = m_lEmitters[selected].particles[i].velIndices[a];
                            m_lEmitters[selected].particles[i].velIndices[a] = m_lEmitters[selected].particles[i].velIndices[b];
                            m_lEmitters[selected].particles[i].velIndices[b] = swapt;

                        }
                    }
                }

            }
        }
                      
        private void lbParticles_SelectedIndexChanged(object sender, EventArgs e)
        {
            particleChange();
        }

        //
        // Bool Checks
        //
        public bool partIsSelected()
        {
            if (lbParticles.SelectedIndex >= 0 && lbParticles.SelectedIndex < lbParticles.Items.Count)            
                return true;
            else
                return false;
        }

        public bool colorIsSelected()
        {
            if (lbColors.SelectedIndex >= 0 && lbColors.SelectedIndex < lbColors.Items.Count)            
                return true;
            else
                return false;
        }

        public bool sizeIsSelected()
        {
            if (lbSizes.SelectedIndex >= 0 && lbSizes.SelectedIndex < lbSizes.Items.Count)
                return true;
            else
                return false;
        }

        public bool velIsSelected()
        {
            if (lbVelocities.SelectedIndex >= 0 && lbVelocities.SelectedIndex < lbVelocities.Items.Count)
                return true;
            else
                return false;
        }



        private void LoadImage(int emitterIndex)
        {
            if (emitterIsSelected())
            {
                OpenFileDialog dlg = new OpenFileDialog();

                if (Directory.Exists(currentPath + "\\Textures\\"))
                    dlg.InitialDirectory = currentPath + "\\Textures\\";
                else
                    dlg.InitialDirectory = currentPath;


                dlg.Filter = "png files (*.png)|*.png|bmp files (*.bmp)|*bmp|jpeg files (*.jpg)|*.jpe|All files (*.*)|*.*";
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    currentPath = Path.GetDirectoryName(dlg.FileName);

                    if (emitterIndex >= 0)
                    {
                        m_lEmitters[emitterIndex].imageID = ManagedTextureManager.Instance.LoadTexture(dlg.FileName, Color.White.ToArgb());
                        m_nID = ManagedTextureManager.Instance.LoadTexture(dlg.FileName, Color.White.ToArgb());
                        m_lEmitters[emitterIndex].filename = Path.GetFileName(dlg.FileName);
                        m_lEmitters[emitterIndex].filepath = Path.GetDirectoryName(dlg.FileName) + "\\";
                    }
                    else if (selected < m_lEmitters.Count() && selected > 0)
                    {
                        m_lEmitters[selected].imageID = ManagedTextureManager.Instance.LoadTexture(dlg.FileName, Color.White.ToArgb());
                        m_nID = ManagedTextureManager.Instance.LoadTexture(dlg.FileName, Color.White.ToArgb());
                        m_lEmitters[selected].filename = Path.GetFileName(dlg.FileName);
                        m_lEmitters[selected].filepath = Path.GetDirectoryName(dlg.FileName) + "\\";
                    }
                    imageLocation = dlg.FileName;
                    gameTime = (System.DateTime.Now);
                    pPreview.Invalidate();
                    pTexture.Invalidate();
                    changeSelectedEmitter(selected);
                }

                
                m_lEmitters[selected].reset();
                
            }
            else
            {
                string message = "No Emitter Selected.\n\nSelect an Emitter from the Lefthand list";
                string caption = "Load Image Failed";
                var result = MessageBox.Show(message, caption,
                               MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void lbEmitters_SelectedIndexChanged(object sender, EventArgs e)
        {
            changeSelectedEmitter(lbEmitters.SelectedIndex);
        }

        private void pTexture_Paint(object sender, PaintEventArgs e)
        {
            if ( emitterIsSelected() )
            {
                Bitmap bmp;
                Rectangle r = new Rectangle();
                r.X = 0;
                r.Y = 0;
                r.Width = pTexture.Width;
                r.Height = pTexture.Height;
                if (bSingleImage && File.Exists(imageLocation))
                {
                    bmp = new Bitmap(imageLocation);
                    e.Graphics.DrawImage(bmp, r);
                }
                else if (File.Exists(m_lEmitters[selected].filepath + "\\" + m_lEmitters[selected].filename))
                {
                    bmp = new Bitmap((m_lEmitters[selected].filepath + "\\" + m_lEmitters[selected].filename));
                    e.Graphics.DrawImage(bmp, r);
                }
                else
                    e.Graphics.FillRectangle(Brushes.Black, r);
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();

            dlg.InitialDirectory = currentPath;
            dlg.Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                currentPath = Path.GetDirectoryName(dlg.FileName);
                
                SaveXml(dlg.FileName);
            }

        }

        private void openXMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.InitialDirectory = currentPath;
            
            dlg.Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                lbEmitters.Items.Clear();
                for( int i = 0; i < m_lEmitters.Count(); ++i)
                    //ManagedTextureManager.Instance.ReleaseTexture(m_lEmitters[i].imageID);
                m_lEmitters.Clear();

                currentPath = Path.GetDirectoryName(dlg.FileName);
                //System.Uri uri1 = new Uri(currentPath);
                //System.Uri uri2 = new Uri(dsafsdf);
                //Uri relative = uri1.MakeRelativeUri(uri2);
                //string s = relative.ToString();
                //for (int i = 0; i < m_lEmitters.Count(); ++i)
                //{
                //    if (m_lEmitters[i].imageID >= 0)
                //        ManagedTextureManager.Instance.ReleaseTexture(m_lEmitters[i].imageID);
                //}
                LoadXml(dlg.FileName);
                
            }
        }

        public void LoadXml(string path)
        {
            XElement root = XElement.Load(path);

            IEnumerable<XElement> emitters = root.Elements();

            foreach (XElement emitter in emitters)
            {
                XAttribute tryTo = emitter.Attribute("Name");

                if (tryTo != null)
                {
                    particleEmitter pe = new particleEmitter();


                    pe.name = emitter.Attribute("Name").Value;
                    pe.filepath = Path.GetDirectoryName(path) + "\\Textures\\";
                    //pe.filepath += emitter.Attribute("ImageName").Value;
                    pe.filename = emitter.Attribute("ImageName").Value;
                    pe.eDuration = (float)Convert.ToDouble((emitter.Attribute("Duration").Value));
                    pe.pDuration = (float)(Convert.ToDouble(emitter.Attribute("ParticleDuration").Value));
                    pe.isLooping = Convert.ToBoolean(Convert.ToInt32((emitter.Attribute("IsLooping").Value)));
                    pe.isBurst = Convert.ToBoolean(Convert.ToInt32((emitter.Attribute("IsBurst").Value)));
                    pe.force.X = (float)Convert.ToDouble(emitter.Attribute("xForce").Value);
                    pe.force.Y = (float)Convert.ToDouble(emitter.Attribute("yForce").Value);
                    pe.pRect.X = pPreview.Width / 2;
                    pe.pRect.Y = pPreview.Height / 2;
                    pe.pRect.Width = Convert.ToInt32(emitter.Attribute("rectWidth").Value);
                    pe.pRect.Height = Convert.ToInt32(emitter.Attribute("rectHeight").Value);
                    pe.radius = (float)Convert.ToDouble(emitter.Attribute("Radius").Value);
                    pe.shape = Convert.ToInt32(emitter.Attribute("Shape").Value);
                    pe.sourceBlendMode = Convert.ToInt32(emitter.Attribute("SourceBlendMode").Value);
                    pe.destBlendMode = Convert.ToInt32(emitter.Attribute("DestinationBlendMode").Value);
                    pe.particleScalar = (float)Convert.ToDouble(emitter.Attribute("ParticleScalar").Value);
                    pe.pos.X = pPreview.Width / 2.0f;
                    pe.pos.Y = pPreview.Height / 2.0f;
                    pe.startPos = pe.pos;
                    


                    //if (doc.Name.ToString() == "ParticleEmitters")
                    IEnumerable<XElement> uColorsRoot = emitter.Elements();

                    IEnumerable<XElement> uColors = uColorsRoot.Elements();


                    foreach (XElement color in uColors)
                    {
                        XAttribute tryIt = color.Attribute("Alpha");
                        if (tryIt != null)
                        {
                            tColorStop tc = new tColorStop(1);
                            tc.color.Alpha = (float)Convert.ToDouble(color.Attribute("Alpha").Value);
                            tc.color.Red = (float)Convert.ToDouble(color.Attribute("Red").Value);
                            tc.color.Green = (float)Convert.ToDouble(color.Attribute("Green").Value);
                            tc.color.Blue = (float)Convert.ToDouble(color.Attribute("Blue").Value);

                            tc.coef = (float)Convert.ToDouble(color.Attribute("Coeficient").Value);

                            pe.uColors.Add(tc);
                        }
                    }

                    IEnumerable<XElement> uSizesRoot = emitter.Elements("UniqueSizes");

                    IEnumerable<XElement> uSizes = uSizesRoot.Elements();

                    foreach (XElement size in uSizes)
                    {
                        XAttribute tryIt = size.Attribute("Width");
                        if (tryIt != null)
                        {
                            tSizeStop ts = new tSizeStop(1);
                            ts.scale.X = (float)Convert.ToDouble(size.Attribute("Width").Value);
                            ts.scale.Y = (float)Convert.ToDouble(size.Attribute("Height").Value);

                            ts.coef = (float)Convert.ToDouble(size.Attribute("Coeficient").Value);

                            pe.uSizes.Add(ts);
                        }
                    }

                    IEnumerable<XElement> uVelsRoot = emitter.Elements("UniqueVelocities");

                    IEnumerable<XElement> uVels = uVelsRoot.Elements();

                    foreach (XElement vel in uVels)
                    {
                        XAttribute tryIt = vel.Attribute("xVelocity");
                        if (tryIt != null)
                        {
                            tVelStop tv = new tVelStop(1);
                            tv.vel.X = (float)Convert.ToDouble(vel.Attribute("xVelocity").Value);
                            tv.vel.Y = (float)Convert.ToDouble(vel.Attribute("yVelocity").Value);

                            tv.coef = (float)Convert.ToDouble(vel.Attribute("Coeficient").Value);

                            pe.uVels.Add(tv);
                        }
                    }

                    IEnumerable<XElement> particlesRoot = emitter.Elements("Particles");

                    IEnumerable<XElement> particles = particlesRoot.Elements();

                    foreach (XElement part in particles)
                    {

                        if (part.Element("ColorIndices") != null)
                        {
                            tParticle p = new tParticle(1);

                            IEnumerable<XElement> cIndicesRoot = part.Elements();

                            IEnumerable<XElement> cIndices = cIndicesRoot.Elements("Index");

                            foreach (XElement i in cIndices)
                            {
                                XAttribute tryIt = i.Attribute("ci");
                                if (tryIt != null)
                                {
                                    int Index = new int();
                                    Index = Convert.ToInt32(i.Attribute("ci").Value);
                                    p.colorIndices.Add(Index);
                                }
                            }

                            IEnumerable<XElement> sIndicesRoot = part.Elements();

                            IEnumerable<XElement> sIndices = sIndicesRoot.Elements("Index");

                            //IEnumerable<XElement> ind = sIndicesRoot.Descendants();

                            foreach (XElement i in sIndices)
                            {
                                XAttribute tryIt = i.Attribute("si");
                                if (tryIt != null)
                                {
                                    int Index = new int();
                                    Index = Convert.ToInt32(i.Attribute("si").Value);
                                    p.sizeIndices.Add(Index);
                                }
                            }

                            IEnumerable<XElement> vIndicesRoot = part.Elements();

                            IEnumerable<XElement> vIndices = vIndicesRoot.Elements();

                            foreach (XElement i in vIndices)
                            {
                                XAttribute tryIt = i.Attribute("vi");
                                if (tryIt != null)
                                {
                                    int Index = new int();
                                    Index = Convert.ToInt32(i.Attribute("vi").Value);
                                    p.velIndices.Add(Index);
                                }
                            }

                            pe.particles.Add(p);
                        }
                    }

                    m_lEmitters.Add(pe);
                }

            }

            lbEmitters.Items.Clear();
            for (int i = 0; i < m_lEmitters.Count; ++i)
            {
                string mypath = currentPath + "\\Textures\\" + m_lEmitters[i].filename;
                if(File.Exists(mypath))
                    m_lEmitters[i].imageID = ManagedTextureManager.Instance.LoadTexture( mypath, Color.White.ToArgb());
                lbEmitters.Items.Add(m_lEmitters[i].name);
            }
            if (m_lEmitters.Count > 0)
            {
                //changeSelectedEmitter(0);
            }

        }

        public void SaveXml(string path)
        {
            XElement xRoot = new XElement("ParticleEmitters");

            xRoot.Add(new XAttribute("NumberOfEmitters", m_lEmitters.Count().ToString()));

            foreach( particleEmitter pe in m_lEmitters)
            {
                XElement xEmitter = new XElement("Emitter");

                xEmitter.Add(new XAttribute("Name", pe.name));
                xEmitter.Add(new XAttribute("ImageName", pe.filename));
                xEmitter.Add(new XAttribute("Duration", pe.eDuration));
                xEmitter.Add(new XAttribute("ParticleDuration", pe.pDuration));
                xEmitter.Add(new XAttribute("IsLooping", Convert.ToInt32(pe.isLooping)));
                xEmitter.Add(new XAttribute("IsBurst", Convert.ToInt32(pe.isBurst)));
                xEmitter.Add(new XAttribute("xForce", pe.force.X));
                xEmitter.Add(new XAttribute("yForce", pe.force.Y));
                xEmitter.Add(new XAttribute("rectWidth", pe.pRect.Width));
                xEmitter.Add(new XAttribute("rectHeight", pe.pRect.Height));
                xEmitter.Add(new XAttribute("Radius", pe.radius));
                xEmitter.Add(new XAttribute("Shape", Convert.ToInt32(pe.shape)));
                xEmitter.Add(new XAttribute("SourceBlendMode", pe.sourceBlendMode));
                xEmitter.Add(new XAttribute("DestinationBlendMode", pe.destBlendMode));
                xEmitter.Add(new XAttribute("ParticleScalar", pe.particleScalar));

                XElement uc = new XElement("UniqueColors");
                foreach( tColorStop cs in pe.uColors )
                {
                    XElement c = new XElement("Color");
                    c.Add(new XAttribute("Alpha", cs.color.Alpha));
                    c.Add(new XAttribute("Red", cs.color.Red));
                    c.Add(new XAttribute("Green", cs.color.Green));
                    c.Add(new XAttribute("Blue", cs.color.Blue));
                    c.Add(new XAttribute("Coeficient", cs.coef));
                    uc.Add(c);
                }
                xEmitter.Add(uc);

                XElement us = new XElement("UniqueSizes");
                foreach (tSizeStop ts in pe.uSizes)
                {
                    XElement s = new XElement("Size");
                    s.Add(new XAttribute("Width", ts.scale.X));
                    s.Add(new XAttribute("Height", ts.scale.Y));
                    s.Add(new XAttribute("Coeficient", ts.coef));
                    us.Add(s);
                }
                xEmitter.Add(us);

                XElement uv = new XElement("UniqueVelocities");
                foreach (tVelStop vs in pe.uVels)
                {
                    XElement v = new XElement("Velocity");
                    v.Add(new XAttribute("xVelocity", vs.vel.X));
                    v.Add(new XAttribute("yVelocity", vs.vel.Y));
                    v.Add(new XAttribute("Coeficient", vs.coef));
                    uv.Add(v);
                }
                xEmitter.Add(uv);

                XElement parts = new XElement("Particles");
                parts.Add(new XAttribute("NumberOfParticles", pe.particles.Count()));

                foreach( tParticle p in pe.particles )
                {
                    XElement xp = new XElement("Particle");

                    
                    XElement ci = new XElement("ColorIndices");
                    foreach (int i in p.colorIndices)
                    {
                        XElement xi = new XElement("Index");
                        xi.Add(new XAttribute("ci", i));
                        ci.Add(xi);
                    }
                    xp.Add(ci);

                    XElement si = new XElement("SizeIndices");
                    foreach (int i in p.sizeIndices)
                    {
                        XElement xi = new XElement("Index");
                        xi.Add(new XAttribute("si", i));
                        si.Add(xi);
                    }
                    xp.Add(si);

                    XElement vi = new XElement("VelocityIndices");
                    foreach (int i in p.velIndices)
                    {
                        XElement xi = new XElement("Index");
                        xi.Add(new XAttribute("vi", i));
                        vi.Add(xi);
                    }
                    xp.Add(vi);


                    parts.Add(xp);
                }
                             
                
                xEmitter.Add(parts);

                xRoot.Add(xEmitter);
            }

            xRoot.Save(path);

        }

        private void bRandom_Click(object sender, EventArgs e)
        {
            //LoadXml(Directory.GetCurrentDirectory() + "\\Particles.xml");
            if (emitterIsSelected())
            {
                RandomizeEmitter();
            }
        }

        private void cbBlendSource_SelectedIndexChanged(object sender, EventArgs e)
        {
            if( emitterIsSelected() )
            {
                m_lEmitters[selected].sourceBlendMode = cbBlendSource.SelectedIndex+1;
            }
        }

        private void cbBlendDestination_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].destBlendMode = cbBlendDestination.SelectedIndex + 1;
            }
        }

        private void pPreview_MouseMove(object sender, MouseEventArgs e)
        {
            if (emitterIsSelected())
            {
                if (e.Button.Equals(MouseButtons.Left))
                {
                    m_lEmitters[selected].pos.X = e.Location.X;
                    m_lEmitters[selected].pos.Y = e.Location.Y;
                }
            }
        }
               
        private void cbShape_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].shape = cbShape.SelectedIndex;

                if (cbShape.SelectedIndex == 0)
                {
                    labelWidth.Visible = true;
                    labelHeight.Visible = true;
                    nuWidth.Visible = true;
                    nuHeight.Visible = true;
                    //nuWidth.Value = 100;
                    //nuHeight.Value = 100;
                    labelRadius.Visible = false;
                    nuRadius.Visible = false;
                }
                else if (cbShape.SelectedIndex == 1)
                {
                    labelRadius.Visible = true;
                    nuRadius.Visible = true;
                    //nuRadius.Value = 100;
                    labelWidth.Visible = false;
                    labelHeight.Visible = false;
                    nuWidth.Visible = false;
                    nuHeight.Visible = false;
                }
                else
                {
                    labelWidth.Visible = false;
                    labelHeight.Visible = false;
                    nuWidth.Visible = false;
                    nuHeight.Visible = false;
                    labelRadius.Visible = false;
                    nuRadius.Visible = false;
                }

                if (cbShape.SelectedIndex == 0)
                {
                    decimal width = m_lEmitters[selected].pRect.Width;
                    decimal height = m_lEmitters[selected].pRect.Height;
                    nuWidth.Value = width = (width > 0) ? (decimal)width : 100;
                    nuHeight.Value = height = (width > 0) ? (decimal)height : 100;
                    m_lEmitters[selected].pRect.Width = (int)width;
                    m_lEmitters[selected].pRect.Height = (int)height;
                }
                else if (cbShape.SelectedIndex == 1)
                {
                    decimal radius = (decimal)m_lEmitters[selected].radius;
                    nuRadius.Value = radius = (radius > 0) ? radius : 100;
                    m_lEmitters[selected].pRect.X = (int)m_lEmitters[selected].pos.X - (int)radius;
                    m_lEmitters[selected].pRect.Y = (int)m_lEmitters[selected].pos.Y - (int)radius;
                    m_lEmitters[selected].pRect.Width = (int)radius*2;
                    m_lEmitters[selected].pRect.Height = (int)radius*2;
                }

                m_lEmitters[selected].reset();
            }
        }

        private void nuRadius_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].radius = (float)nuRadius.Value;
                if (cbShape.SelectedIndex == 1)
                {
                    m_lEmitters[selected].pRect.X = (int)m_lEmitters[selected].pos.X - (int)nuRadius.Value;
                    m_lEmitters[selected].pRect.Y = (int)m_lEmitters[selected].pos.Y - (int)nuRadius.Value;
                    m_lEmitters[selected].pRect.Width = (int)nuRadius.Value*2;
                    m_lEmitters[selected].pRect.Height = (int)nuRadius.Value*2;
                    
                }
            }
        }

        private void pPreview_MouseClick(object sender, MouseEventArgs e)
        {
            if (emitterIsSelected())
            {
                if (m_lEmitters[selected].age > m_lEmitters[selected].eDuration &&
                    m_lEmitters[selected].isLooping == false)
                    m_lEmitters[selected].reset();
                m_lEmitters[selected].pos.X = e.Location.X;
                m_lEmitters[selected].pos.Y = e.Location.Y;
                
            }
        }


        private void addEmitter(string name)
        {
            particleEmitter pe = new particleEmitter();
            if (name == "")
                pe.name = "New Emitter";
            else
                pe.name = name;

            pe.pDuration = 2.0f;
            pe.eDuration = 20.0f;
            pe.isLooping = true;
            pe.isBurst = false;// Convert.ToBoolean(rand.Next(0, 1));
            pe.force = new Microsoft.DirectX.Vector2(0, 0);


            bool simple = true;
            if (simple)
            {
                //addParticle();
                //addParticle();
                tColorStop t = new tColorStop(1);
                t.coef = 0.0f;
                t.color = new ColorValue(1.0f, 0.0f, 0.0f, 1.0f);
                pe.uColors.Add(t);

                t = new tColorStop(1);
                t.coef = 0.5f;
                t.color = new ColorValue(0.0f, 1.0f, 0.0f, 1.0f);
                pe.uColors.Add(t);

                t = new tColorStop(1);
                t.coef = 1.0f;
                t.color = new ColorValue(0.0f, 0.0f, 1.0f, 1.0f);
                pe.uColors.Add(t);

                tSizeStop s = new tSizeStop(1);
                s.coef = 0.0f;
                s.scale = new Microsoft.DirectX.Vector2(0.5f, 0.5f);
                pe.uSizes.Add(s);

                s = new tSizeStop(1);
                s.coef = 1.0f;
                s.scale = new Microsoft.DirectX.Vector2(1.0f, 1.0f);
                pe.uSizes.Add(s);

                tVelStop v = new tVelStop(1);
                v.coef = 0.0f;
                v.vel = new Microsoft.DirectX.Vector2(30, -30);
                pe.uVels.Add(v);

                v = new tVelStop(1);
                v.coef = 0.0f;
                v.vel = new Microsoft.DirectX.Vector2(-30, -30);
                pe.uVels.Add(v);

                v = new tVelStop(1);
                v.coef = 0.25f;
                v.vel = new Microsoft.DirectX.Vector2(0, -10);
                pe.uVels.Add(v);

                tParticle p = new tParticle(1);
                p.colorIndices.Add(0);
                p.colorIndices.Add(1);
                p.colorIndices.Add(2);
                p.sizeIndices.Add(0);
                p.sizeIndices.Add(1);
                p.velIndices.Add(0);
                p.velIndices.Add(2);
                pe.particles.Add(p);

                p = new tParticle(1);
                p.colorIndices.Add(0);
                p.colorIndices.Add(2);
                p.sizeIndices.Add(0);
                p.sizeIndices.Add(1);
                p.velIndices.Add(1);
                p.velIndices.Add(2);
                pe.particles.Add(p);

            }
            else
            {

                int distribution = 5;
                int subFactor = 3;
                for (int i = 0; i < distribution; ++i)
                {
                    for (int x = 0; x < subFactor; ++x)
                    {
                        tColorStop c = new tColorStop(1);
                        c.coef = i * (1.0f / distribution);
                        //c.color = new ColorValue(rand.Next(0, 10) / 10.0f, rand.Next(0, 10) / 10.0f,
                        //                    rand.Next(0, 10) / 10.0f, rand.Next(0, 10) / 10.0f);

                        if (i == 0) c.color = new ColorValue(1.0f, 0.0f, 0.0f, 1.0f);
                        if (i == 1) c.color = new ColorValue(0.0f, 1.0f, 0.0f, 1.0f);

                        pe.uColors.Add(c);

                        tSizeStop s = new tSizeStop(1);
                        s.coef = i * (1.0f / distribution);
                        s.scale = new Microsoft.DirectX.Vector2(rand.Next(1, 10) / 10.0f, rand.Next(1, 10) / 10.0f);
                        pe.uSizes.Add(s);


                        tVelStop v = new tVelStop(1);
                        v.coef = i * (1.0f / distribution);
                        v.vel = new Microsoft.DirectX.Vector2(rand.Next(-100, 100), rand.Next(-100, 100));
                        pe.uVels.Add(v);
                    }
                }

                for (int i = 0; i < 100; ++i)
                {
                    tParticle p = new tParticle(1);
                    p.width = 1.0f;
                    p.height = 1.0f;
                    p.age = 0.0f;


                    p.colorIndices.Add(rand.Next(0, subFactor - 1));
                    //p.colorIndices.Add(0);
                    for (int x = 0; x < 10; ++x)
                        p.colorIndices.Add(rand.Next(subFactor, subFactor * distribution - subFactor - 1));
                    p.colorIndices.Add(rand.Next(subFactor * distribution - subFactor, subFactor * distribution));

                    p.sizeIndices.Add(rand.Next(0, subFactor - 1));
                    for (int x = 0; x < 10; ++x)
                        p.sizeIndices.Add(rand.Next(subFactor, subFactor * distribution - subFactor - 1));
                    p.sizeIndices.Add(rand.Next(subFactor * distribution - subFactor, subFactor * distribution));

                    p.velIndices.Add(rand.Next(0, subFactor - 1));
                    for (int x = 0; x < 10; ++x)
                        p.velIndices.Add(rand.Next(subFactor, subFactor * distribution - subFactor - 1));
                    p.velIndices.Add(rand.Next(subFactor * distribution - subFactor, subFactor * distribution));

                    pe.particles.Add(p);
                }
            }

            m_lEmitters.Add(pe);
            lbEmitters.Items.Add(pe.name);
            //lbEmitters.SelectedIndex = 0;
            //addParticle();
            //addParticle();
            
        }

        private void tbName_KeyDown(object sender, KeyEventArgs e)
        {
            if ( emitterIsSelected() && e.KeyCode == Keys.Enter)
            {
                m_lEmitters[selected].name = tbName.Text;
                lbEmitters.Items[selected] = tbName.Text;
            }

        }

        private void bLoadTexture_Click(object sender, EventArgs e)
        {
                LoadImage(selected);
        }

        private void pTexture_Click(object sender, EventArgs e)
        {
                LoadImage(selected);
        }

        private void checkBox9_CheckedChanged(object sender, EventArgs e)
        {
            
            bSingleImage = !bSingleImage;
            if (emitterIsSelected() && bSingleImage && m_nID != -1 )
            {
                ManagedTextureManager m = ManagedTextureManager.Instance;
                imageLocation = m_lEmitters[selected].filepath + m_lEmitters[selected].filename;
                m_nID = m.LoadTexture(imageLocation, new ColorValue(1,1,1,1).ToArgb());//m_lEmitters[selected].imageID;
            }
        }

        private void bAddEmitter_Click(object sender, EventArgs e)
        {
            addEmitter(tbName.Text);
        }

        

        private void lbColors_SelectedIndexChanged(object sender, EventArgs e)
        {
            colorChange();
            gbColorStop.Visible = true;
        }

        private void colorChange()
        {
            if (colorIsSelected())
            {
                // Get the Color and life and display it 
                ColorValue c = m_lEmitters[selected].uColors[(int)lbColors.SelectedItem].color;
                Color newColor = new Color();
                newColor = Color.FromArgb(c.ToArgb());
                bChangeColor.BackColor = newColor;
                int coef = (int)(m_lEmitters[selected].uColors[(int)lbColors.SelectedItem].coef * 100.0f);
                trackBarColorTime.Value = coef;
            }

            //bChangeColor.BackColor = m_lEmitters[selected].particles[ ].
        }

        private void lbSizes_SelectedIndexChanged(object sender, EventArgs e)
        {
            sizeChange();
            gbSizeStop.Visible = true;
        }

        private void sizeChange()
        {
            if (sizeIsSelected())
            {
                 // Get The size and Life and display them
                nuSizeWidth.Value = (decimal)m_lEmitters[selected].uSizes[(int)lbSizes.SelectedItem].scale.X;
                nuSizeHeight.Value = (decimal)m_lEmitters[selected].uSizes[(int)lbSizes.SelectedItem].scale.Y;
                int coef = (int)(m_lEmitters[selected].uSizes[(int)lbSizes.SelectedItem].coef * 100.0f);
                trackBarSizeTime.Value = coef;
            }
        }

        private void lbVelocities_SelectedIndexChanged(object sender, EventArgs e)
        {
            velocityChange();
            gbVelStop.Visible = true;
        }

        private void velocityChange()
        {
            if (velIsSelected())
            {
                nuXVelocity.Value = (decimal)m_lEmitters[selected].uVels[(int)lbVelocities.SelectedItem].vel.X;
                nuYVelocity.Value = (decimal)m_lEmitters[selected].uVels[(int)lbVelocities.SelectedItem].vel.Y;
                int coef = (int)(m_lEmitters[selected].uVels[(int)lbVelocities.SelectedItem].coef * 100.0f);
                trackBarVelocityTime.Value = coef;
            }
        }

        private void bChangeColor_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                if (colorSelected())
                {
                    Color c = dlg.Color;
                    m_lEmitters[selected].uColors[(int)lbColors.SelectedItem].color = new ColorValue(c.R, c.G, c.B, c.A);
                    bChangeColor.BackColor = c;
                }
            }
        }

        private void nuSizeWidth_ValueChanged(object sender, EventArgs e)
        {
            if (sizeIsSelected())
            {
                m_lEmitters[selected].uSizes[(int)lbSizes.SelectedItem].scale.X = (float)nuSizeWidth.Value;
            }
        }

        private void nuSizeHeight_ValueChanged(object sender, EventArgs e)
        {
            if (sizeIsSelected())
            {
                m_lEmitters[selected].uSizes[(int)lbSizes.SelectedItem].scale.Y = (float)nuSizeHeight.Value;
            }
        }

        private void nuXVelocity_ValueChanged(object sender, EventArgs e)
        {
            if (velIsSelected())
            {
                m_lEmitters[selected].uVels[(int)lbVelocities.SelectedItem].vel.X = (float)nuXVelocity.Value;
            }
        }

        private void nuYVelocity_ValueChanged(object sender, EventArgs e)
        {
            if (velIsSelected())
            {
                m_lEmitters[selected].uVels[(int)lbVelocities.SelectedItem].vel.Y = (float)nuYVelocity.Value;
            }
        }

        private bool colorSelected()
        {
            if (lbColors.SelectedIndex >= 0 && lbColors.SelectedIndex < lbColors.Items.Count)
                return true;
            else
                return false;
        }


        private void hideParticleControls()
        {
            // Particle
            lParticleList.Visible = false;
            lbParticles.Visible = false;
            bAddParticle.Visible = false;
            bAddTenParticles.Visible = false;
            bRemoveParticle.Visible = false;
            bSelectAllParticles.Visible = false;

            // Color Collection
            lColorCollection.Visible = false;
            lbColors.Visible = false;
            bAddColor.Visible = false;
            bRemoveColor.Visible = false;
            gbColorStop.Visible = false;

            // Size Collection
            lSizeCollection.Visible = false;
            lbSizes.Visible = false;
            bAddSize.Visible = false;
            bRemoveSize.Visible = false;
            gbSizeStop.Visible = false;

            // Velocity Collection
            lVelocityCollection.Visible = false;
            lbVelocities.Visible = false;
            bAddVelocity.Visible = false;
            bRemoveVelocity.Visible = false;
            gbVelStop.Visible = false;

        }

        private void bRemoveEmitter_Click(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                string message = "Deleting this emitter is permanent.\nAre you sure?";
                string caption = "Delete Confirmation";
                var result = MessageBox.Show(message, caption,
                               MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                if (result == DialogResult.Yes)
                {
                    lbEmitters.Items.Remove(m_lEmitters[selected].name);
                    m_lEmitters.Remove(m_lEmitters[selected]);
                }
            }
        }

        private void bAddColor_Click(object sender, EventArgs e)
        {
            tColorStop c = new tColorStop(1);
            c.color = new ColorValue(1.0f,1.0f,1.0f,1.0f);
            c.coef = 0.0f;
            m_lEmitters[selected].uColors.Add(c);

            int index = m_lEmitters[selected].uColors.Count() - 1;
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                m_lEmitters[selected].particles[i].colorIndices.Add(index);
            }
            lbColors.Items.Add(index);
        }

        private void bRemoveColor_Click(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                if (lbColors.SelectedIndex >= 0 && lbColors.SelectedIndex < lbColors.Items.Count)
                {
                    int removeMe = (int)lbColors.SelectedItem;
                    for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
                    {
                        while (m_lEmitters[selected].particles[i].colorIndices.Remove(removeMe)) /* do nothing */;
                    }
                    lbColors.Items.Remove(removeMe);
                }
            }
        }

        private void bAddSize_Click(object sender, EventArgs e)
        {
            tSizeStop s = new tSizeStop(1);
            s.scale = new Microsoft.DirectX.Vector2(1.0f, 1.0f);
            s.coef = 0.0f;
            m_lEmitters[selected].uSizes.Add(s);

            int index = m_lEmitters[selected].uSizes.Count() - 1;
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                m_lEmitters[selected].particles[i].sizeIndices.Add(index);
            }
            lbSizes.Items.Add(index);
        }

        private void bRemoveSize_Click(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                if (lbSizes.SelectedIndex >= 0 && lbSizes.SelectedIndex < lbSizes.Items.Count)
                {
                    int removeMe = (int)lbSizes.SelectedItem;
                    for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
                    {
                        while (m_lEmitters[selected].particles[i].sizeIndices.Remove(removeMe)) /* do nothing */;
                    }
                    lbSizes.Items.Remove(removeMe);
                }
            }
        }

        private void bAddVelocity_Click(object sender, EventArgs e)
        {
            tVelStop v = new tVelStop(1);
            v.vel = new Microsoft.DirectX.Vector2(0.0f, 10.0f);
            v.coef = 0.0f;
            m_lEmitters[selected].uVels.Add(v);

            int index = m_lEmitters[selected].uVels.Count() - 1;
            for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
            {
                if (lbParticles.SelectedIndices.Contains(i) == false) continue;

                m_lEmitters[selected].particles[i].velIndices.Add(index);
            }
            lbVelocities.Items.Add(index);
        }

        private void bRemoveVelocity_Click(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                if (lbVelocities.SelectedIndex >= 0 && lbVelocities.SelectedIndex < lbVelocities.Items.Count)
                {
                    int removeMe = (int)lbVelocities.SelectedItem;
                    for (int i = 0; i < m_lEmitters[selected].particles.Count(); ++i)
                    {
                        while (m_lEmitters[selected].particles[i].velIndices.Remove(removeMe)) /* do nothing */;
                    }
                    lbVelocities.Items.Remove(removeMe);
                }
            }
        }

        private void bAddParticle_Click(object sender, EventArgs e)
        {
            addParticle();   
        }

        private void addParticle()
        {
            if (emitterIsSelected())
            {
                tColorStop t = new tColorStop(1);
                t.coef = 0.0f;
                t.color = new ColorValue(1.0f, 0.0f, 0.0f, 1.0f);
                if(m_lEmitters[selected].getColorIndex(t) == -1)
                    m_lEmitters[selected].uColors.Add(t);

                t = new tColorStop(1);
                t.coef = 0.5f;
                t.color = new ColorValue(0.0f, 1.0f, 0.0f, 1.0f);
                if (m_lEmitters[selected].getColorIndex(t) == -1)
                    m_lEmitters[selected].uColors.Add(t);

                t = new tColorStop(1);
                t.coef = 1.0f;
                t.color = new ColorValue(0.0f, 0.0f, 1.0f, 1.0f);
                if (m_lEmitters[selected].getColorIndex(t) == -1)
                    m_lEmitters[selected].uColors.Add(t);

                tSizeStop s = new tSizeStop(1);
                s.coef = 0.0f;
                s.scale = new Microsoft.DirectX.Vector2(0.5f, 0.5f);
                if (m_lEmitters[selected].getSizeIndex(s) == -1)
                    m_lEmitters[selected].uSizes.Add(s);

                s = new tSizeStop(1);
                s.coef = 0.5f;
                s.scale = new Microsoft.DirectX.Vector2(1.0f, 1.0f);
                if (m_lEmitters[selected].getSizeIndex(s) == -1)
                    m_lEmitters[selected].uSizes.Add(s);

                s = new tSizeStop(1);
                s.coef = 1.0f;
                s.scale = new Microsoft.DirectX.Vector2(0.5f, 0.5f);
                if (m_lEmitters[selected].getSizeIndex(s) == -1)
                    m_lEmitters[selected].uSizes.Add(s);

                tVelStop v = new tVelStop(1);
                v.coef = 0.0f;
                v.vel = new Microsoft.DirectX.Vector2(30, -30);
                if (m_lEmitters[selected].getVelocityIndex(v) == -1)
                    m_lEmitters[selected].uVels.Add(v);

                v = new tVelStop(1);
                v.coef = 0.0f;
                v.vel = new Microsoft.DirectX.Vector2(-30, -30);
                if (m_lEmitters[selected].getVelocityIndex(v) == -1)
                    m_lEmitters[selected].uVels.Add(v);

                v = new tVelStop(1);
                v.coef = 0.25f;
                v.vel = new Microsoft.DirectX.Vector2(0, -10);
                if (m_lEmitters[selected].getVelocityIndex(v) == -1)
                    m_lEmitters[selected].uVels.Add(v);

                tParticle p = new tParticle(1);
                p.colorIndices.Add(0);
                p.colorIndices.Add(1);
                p.colorIndices.Add(2);
                p.sizeIndices.Add(0);
                p.sizeIndices.Add(1);
                p.velIndices.Add(0);
                p.velIndices.Add(1);
                p.velIndices.Add(2);
                m_lEmitters[selected].particles.Add(p);
                lbParticles.Items.Add(lbParticles.Items.Count);
                m_lEmitters[selected].reset();
            }
            //lbParticles.Items.Add("Particle" + m_lEmitters[selected].particles.Count());
            
        }

        private void bRemoveParticle_Click(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                if (lbParticles.SelectedIndex >= 0 && lbParticles.SelectedIndex < lbParticles.Items.Count)
                {
                    m_lEmitters[selected].particles.Remove(m_lEmitters[selected].particles[lbParticles.SelectedIndex]);
                    lbParticles.Items.Remove(lbParticles.SelectedItem);
                    m_lEmitters[selected].reset();
                }

                for (int i = 0; i < lbParticles.Items.Count; ++i)
                {
                    if ((int)lbParticles.Items[i] != i)
                    {
                        lbParticles.Items[i] = i;
                    }
                }

            }


        }

        private void bAddTenParticles_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 10; ++i)
                addParticle();
        }

        private void bSelectAllParticles_Click(object sender, EventArgs e)
        {
            if (lbParticles.Visible && lbParticles.Items.Count >= 1)
            {
                for(int i = 0; i < lbParticles.Items.Count; ++i)
                    lbParticles.SetSelected(i, true);
            }
        }

        private void cbLooping_CheckedChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].isLooping = cbLooping.Checked;
                m_lEmitters[selected].reset();
            }
        }

        private void cbBurst_CheckedChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].isBurst = cbBurst.Checked;
                m_lEmitters[selected].reset();
            }
        }

        private void nuNumParticles_ValueChanged(object sender, EventArgs e)
        {
            if(emitterIsSelected())
            {
                int difference = m_lEmitters[selected].particles.Count - (int)nuNumParticles.Value;
                if (difference > 0)
                {
                    for (int i = 0; i < difference; ++i )
                        addParticle();
                }
                else if (difference < 0)
                {
                    for(int i = 0; i < Math.Abs(difference); ++i)
                    {
                        lbParticles.Items.RemoveAt(lbParticles.Items.Count-i);
                        //m_lEmitters[selected].p
                    }
                }
            }
        }

        private void trackBarColorTime_Scroll(object sender, EventArgs e)
        {

        }

        private void nuEmitterDuration_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].eDuration = (float)nuEmitterDuration.Value;
            }
        }

        private void nuParticleDuration_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].pDuration = (float)nuParticleDuration.Value;
            }
        }

        private void nuExternalXVelocity_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].force.X = (float)nuExternalXVelocity.Value;
            }
        }

        private void nuExternalYVelocity_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].force.Y = (float)nuExternalYVelocity.Value;
            }
        }

        private void RandomizeEmitter()
        {
            if (cbLoopingRand.Checked)
                m_lEmitters[selected].isLooping = (rand.Next(0, 100) <= 50) ? false : true;
            if (cbBurstRand.Checked)
                m_lEmitters[selected].isBurst = (rand.Next(1, 100) <= 50) ? false : true;

            if (cbBlendModeRand.Checked)
            {
                m_lEmitters[selected].sourceBlendMode = rand.Next(2, 15);
                m_lEmitters[selected].destBlendMode = rand.Next(2, 15);
            }

            if (cbEmitterDuration.Checked)
                m_lEmitters[selected].eDuration = rand.Next(1, 30);

            if (cbParticleDurationRand.Checked)
                m_lEmitters[selected].pDuration = rand.Next(1, 70)/10.0f;

            if (cbExternalForceRand.Checked)
            {
                m_lEmitters[selected].force.X = rand.Next(-200, 200) / 10.0f;
                m_lEmitters[selected].force.Y = rand.Next(-200, 200) / 10.0f;
            }

            if (cbShapeRand.Checked)
            {
                m_lEmitters[selected].shape = rand.Next(1, 4) - 1;
            }

            int numParts = m_lEmitters[selected].particles.Count();
            if (cbNumParticlesRand.Checked)
                numParts = rand.Next(numParts, numParts + numParts / 2);

            randCurColors();
            randCurSizes();
            randCurVels();
            

            //for (int i = 0; i < numParts; ++i)
            //{
            //    int numStops = m_lEmitters[selected].uColors.Count();

            //}

            changeSelectedEmitter(selected);
        }

        private void randCurColors()
        {
            foreach (tColorStop c in m_lEmitters[selected].uColors)
            {
                if(cbColorRand.Checked)
                    c.color = new ColorValue((rand.Next(1, 101) - 1) / 100.0f, (rand.Next(1, 101) - 1) / 100.0f,
                                          (rand.Next(1, 101) - 1) / 100.0f, (rand.Next(1, 101) - 1) / 100.0f);
                if (cbColorCoefRand.Checked)
                    c.coef = (rand.Next(1, 101) - 1) / 100.0f;
            }
        }

        private void randCurSizes()
        {
            foreach (tSizeStop s in m_lEmitters[selected].uSizes)
            {
                if (cbScaleRand.Checked)
                    s.scale = new Microsoft.DirectX.Vector2(
                        (rand.Next(1, 31) - 1) / 10.0f, (rand.Next(1, 31) - 1) / 10.0f);

                if (cbScaleCoefRand.Checked)
                    s.coef = (rand.Next(1, 101) - 1) / 100.0f;                    
            }
        }

        private void randCurVels()
        {
            foreach (tVelStop v in m_lEmitters[selected].uVels)
            {
                if (cbVelocityRand.Checked)
                    v.vel = new Microsoft.DirectX.Vector2(
                        (rand.Next(-150, 150)), (rand.Next(-150, 150)));

                if (cbVelocityCoefRand.Checked)
                    v.coef = (rand.Next(1, 101) - 1) / 100.0f;
            }
        }

        private void cbColorRand_CheckedChanged(object sender, EventArgs e)
        {

            //if ( ! cbColorRand.Checked)
            //{
            //    cbColorCoefRand.Checked = false;
            //    cbColorStopNumRand.Checked = false;
            //    cbColorCollectionNumRand.Checked = false;
            //    cbColorCoefRand.Visible = false;
            //    cbColorStopNumRand.Visible = false;
            //    cbColorCollectionNumRand.Visible = false;
            //}
            //else
            //{
            //    cbColorCoefRand.Checked = true;
            //    cbColorStopNumRand.Checked = true;
            //    cbColorCollectionNumRand.Checked = true;
            //    cbColorCoefRand.Visible = true;
            //    cbColorStopNumRand.Visible = true;
            //    cbColorCollectionNumRand.Visible = true;
            //}
        }

        private void cbScaleRand_CheckedChanged(object sender, EventArgs e)
        {
            //if (!cbScaleRand.Checked)
            //{
            //    cbScaleCoefRand.Checked = false;
            //    cbScaleCollectionNumRand.Checked = false;
            //    cbScaleStopNumRand.Checked = false;
            //    cbScaleCoefRand.Visible = false;
            //    cbScaleCollectionNumRand.Visible = false;
            //    cbScaleStopNumRand.Visible = false;
            //}
            //else
            //{
            //    cbScaleCoefRand.Checked = true;
            //    cbScaleCollectionNumRand.Checked = true;
            //    cbScaleStopNumRand.Checked = true;
            //    cbScaleCoefRand.Visible = true;
            //    cbScaleCollectionNumRand.Visible = true;
            //    cbScaleStopNumRand.Visible = true;
            //}
        }

        private void cbVelocityRand_CheckedChanged(object sender, EventArgs e)
        {
            //if (!cbVelocityRand.Checked)
            //{
            //    cbVelocityCoefRand.Checked = false;
            //    cbVelocityStopNumRand.Checked = false;
            //    cbVelocityCollectionNumRand.Checked = false;
            //    cbVelocityCoefRand.Visible = false;
            //    cbVelocityStopNumRand.Visible = false;
            //    cbVelocityCollectionNumRand.Visible = false;
            //}
            //else
            //{
            //    cbVelocityCoefRand.Checked = true;
            //    cbVelocityStopNumRand.Checked = true;
            //    cbVelocityCollectionNumRand.Checked = true;
            //    cbVelocityCoefRand.Visible = true;
            //    cbVelocityStopNumRand.Visible = true;
            //    cbVelocityCollectionNumRand.Visible = true;
            //}
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
                LoadImage(selected);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {

            string message = "All changes made since last save will be lost.\n\nContinue?";
            string caption = "New Project";
            var result = MessageBox.Show(message, caption,
                           MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (result == DialogResult.Yes)
            {
                lbColors.Items.Clear();
                lbSizes.Items.Clear();
                lbVelocities.Items.Clear();
                lbParticles.Items.Clear();
                lbEmitters.Items.Clear();
                selected = -1;
                m_lEmitters.Clear();
                hideParticleControls();
            }
            
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void nuWidth_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].pRect.Width = (int)nuWidth.Value;
            }
        }

        private void nuHeight_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].pRect.Height = (int)nuHeight.Value;
            }
        }

        private void randomizeCurrentEmitterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if(emitterIsSelected())
                RandomizeEmitter();
        }

        private void openXMLAndAppendToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            dlg.InitialDirectory = currentPath;

            dlg.Filter = "xml files (*.xml)|*.xml|All files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                //lbEmitters.Items.Clear();
                //for (int i = 0; i < m_lEmitters.Count(); ++i)
                //    //ManagedTextureManager.Instance.ReleaseTexture(m_lEmitters[i].imageID);
                //    m_lEmitters.Clear();

                currentPath = Path.GetDirectoryName(dlg.FileName);
                //System.Uri uri1 = new Uri(currentPath);
                //System.Uri uri2 = new Uri(dsafsdf);
                //Uri relative = uri1.MakeRelativeUri(uri2);
                //string s = relative.ToString();
                //for (int i = 0; i < m_lEmitters.Count(); ++i)
                //{
                //    if (m_lEmitters[i].imageID >= 0)
                //        ManagedTextureManager.Instance.ReleaseTexture(m_lEmitters[i].imageID);
                //}
                LoadXml(dlg.FileName);

            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string message = "Designed and Programmed by Adam Foy at Full Sail University.\nContact Info: foy_adam@yahoo.com";
            string caption = "About me";
            var result = MessageBox.Show(message, caption,
                           MessageBoxButtons.OK, MessageBoxIcon.Information);

        }

        private void bLoadBG_Click(object sender, EventArgs e)
        {
            //if (emitterIsSelected())
            //{
            //    m_lEmitters[selected].sortStops();
            //}
            
            OpenFileDialog dlg = new OpenFileDialog();

            if (Directory.Exists(currentPath + "\\Textures\\"))
                dlg.InitialDirectory = currentPath + "\\Textures\\";
            else
                dlg.InitialDirectory = currentPath;


            dlg.Filter = "png files (*.png)|*.png|bmp files (*.bmp)|*bmp|jpeg files (*.jpg)|*.jpe|All files (*.*)|*.*";
            if (dlg.ShowDialog() == DialogResult.OK)
            {
                nBG_ID = ManagedTextureManager.Instance.LoadTexture(dlg.FileName, Color.White.ToArgb());
                vBGpos = new Vector2(0, 0);
            }
           

        }

     
        private void bUnCheckAllRand_Click(object sender, EventArgs e)
        {
            //Emitter
            cbLoopingRand.Checked = false;
            cbBurstRand.Checked = false;
            cbShapeRand.Checked = false;
            cbBlendModeRand.Checked = false;
            cbEmitterDuration.Checked = false;
            cbParticleDurationRand.Checked = false;
            cbExternalForceRand.Checked = false;
            cbNumParticlesRand.Checked = false;
            //Color
            cbColorRand.Checked = false;
            cbColorCoefRand.Checked = false;
            cbColorCollectionNumRand.Checked = false;
            cbColorStopNumRand.Checked = false;
            //Size
            cbScaleRand.Checked = false;
            cbScaleCoefRand.Checked = false;
            cbScaleCollectionNumRand.Checked = false;
            cbScaleStopNumRand.Checked = false;
            //Velocity
            cbVelocityRand.Checked = false;
            cbVelocityCoefRand.Checked = false;
            cbVelocityCollectionNumRand.Checked = false;
            cbVelocityStopNumRand.Checked = false;

        }

        private void bCheckAllRand_Click(object sender, EventArgs e)
        {
            //Emitter
            cbLoopingRand.Checked = true;
            cbBurstRand.Checked = true;
            cbShapeRand.Checked = true;
            cbBlendModeRand.Checked = true;
            cbEmitterDuration.Checked = true;
            cbParticleDurationRand.Checked = true;
            cbExternalForceRand.Checked = true;
            cbNumParticlesRand.Checked = true;
            //Color
            cbColorRand.Checked = true;
            cbColorCoefRand.Checked = true;
            cbColorCollectionNumRand.Checked = true;
            cbColorStopNumRand.Checked = true;
            //Size
            cbScaleRand.Checked = true;
            cbScaleCoefRand.Checked = true;
            cbScaleCollectionNumRand.Checked = true;
            cbScaleStopNumRand.Checked = true;
            //Velocity
            cbVelocityRand.Checked = true;
            cbVelocityCoefRand.Checked = true;
            cbVelocityCollectionNumRand.Checked = true;
            cbVelocityStopNumRand.Checked = true;
        }

        private void nuScale_ValueChanged(object sender, EventArgs e)
        {
            if (emitterIsSelected())
            {
                m_lEmitters[selected].particleScalar = (float)nuScale.Value;
            }
        }



    }
}

