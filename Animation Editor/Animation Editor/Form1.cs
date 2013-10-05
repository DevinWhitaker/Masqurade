using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml.Linq;
using SGP;

namespace Animation_Editor
{
    enum Brushes { SOURCE, ANCHOR, COLLSION, NUMBRUSHES };
    public partial class Form1 : Form
    {
        ManagedDirect3D D3D = ManagedDirect3D.Instance;
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        List<Animation> animations = new List<Animation>();
        int imageID = -1;
        int currAnimation = -1;
        int PreviewX = 50;
        int PreviewY = 50;
        int ImageX = 0;
        int ImageY = 0;
        int FrameLeft = 0;
        int FrameTop = 0;
        float ImageScale = 1;
        bool looping;
        Color SourceColor = Color.Blue;
        Color EventColor = Color.Red;
        Color AnchorColor = Color.Green;
        Color BackgroungColor = Color.White;
        Brushes mode = Brushes.SOURCE;
        Rectangle source = Rectangle.Empty;
        Rectangle collision = Rectangle.Empty;
        Point anchor = Point.Empty;

        public Form1()
        {
            InitializeComponent();
            D3D.InitManagedDirect3D(ImageWindow, true);
            D3D.InitManagedDirect3D(AnimationPreview, true);
            D3D.InitManagedDirect3D(AnimationPreview2, true);
            TM.InitManagedTextureManager(ManagedDirect3D.Instance.Device, D3D.Sprite);
            looping = true;


            if (Properties.Settings.Default.Events != null)
            {
                for (int i = 0; i < Properties.Settings.Default.Events.Count; i++)
                    EventRectEventComboBox.Items.Add(Properties.Settings.Default.Events[i].ToString());
            }
        }

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        public void Update(float dt)
        {
            if (currAnimation != -1)
            {
                animations[currAnimation].Update(dt);

                if (animations[currAnimation].CurrFrame != -1 && CollisionGroup.Enabled == false)
                    CollisionGroup.Enabled = true;
            }

            if (currAnimation != -1 && FramesListBox.SelectedIndex != -1 && animations[currAnimation].Playing == false)
            {
                SourceGroup.Enabled = true;
                PlayButton.Enabled = true;
                PlayTab2Button.Enabled = true;
                RefreshFramesListBox();
                SourceRectLeftNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Left;
                SourceRectRightNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Right;
                SourceRectTopNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Top;
                SourceRectBottomNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Bottom;
                XAnchorNumeric.Value = animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].X;
                YAnchorNumeric.Value = animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].Y;
            }

            if(currAnimation != -1)
            if (animations[currAnimation].Playing == true)
            {
                PlayButton.Enabled = false;
                PlayTab2Button.Enabled = false;
            }
        }

        public void Render()
        {
            D3D.Clear(ImageWindow, BackgroungColor);
            D3D.DeviceBegin();
            D3D.SpriteBegin();
            {
                if (imageID != -1)
                {
                    TM.Draw(imageID, ImageX, ImageY, ImageScale, ImageScale, Rectangle.Empty, 0, 0, 0, Color.White);
                    D3D.Sprite.Flush();
                }
                if (source != Rectangle.Empty)
                {
                    Rectangle temp = Rectangle.FromLTRB(ImageX + source.Left, ImageY + source.Top, ImageX + source.Right, ImageY + source.Bottom);
                    D3D.DrawEmptyRect(temp, SourceColor);
                }
                if (collision != Rectangle.Empty)
                {
                    Rectangle temp = Rectangle.FromLTRB(ImageX + FrameLeft + collision.Left, ImageY + FrameTop + collision.Top, ImageX + FrameLeft + collision.Right, ImageY + FrameTop + collision.Bottom);
                    D3D.DrawEmptyRect(temp, EventColor);
                }
                

                if (currAnimation != -1)
                {

                    for (int i = 0; i < animations[currAnimation].Frames.Count; ++i)
                    {
                        Rectangle temp = Rectangle.FromLTRB(
                            ImageX + animations[currAnimation].Frames[i].Left,
                            ImageY + animations[currAnimation].Frames[i].Top,
                            ImageX + animations[currAnimation].Frames[i].Right,
                            ImageY + animations[currAnimation].Frames[i].Bottom);

                        D3D.DrawEmptyRect(temp, SourceColor);
                    }

                    for (int i = 0; i < animations[currAnimation].Events.Count; ++i)
                        for (int j = 0; j < animations[currAnimation].Events[i].Count; ++j)
                        {
                            Rectangle temp = Rectangle.FromLTRB(
                                ImageX + (animations[currAnimation].Frames[i].Left + animations[currAnimation].Events[i][j].Rect.Left),
                                ImageY + (animations[currAnimation].Frames[i].Top  + animations[currAnimation].Events[i][j].Rect.Top),
                                ImageX + (animations[currAnimation].Frames[i].Left + animations[currAnimation].Events[i][j].Rect.Right),
                                ImageY + (animations[currAnimation].Frames[i].Top  + animations[currAnimation].Events[i][j].Rect.Bottom));

                            D3D.DrawEmptyRect(temp, EventColor);
                        }

                    for (int i = 0; i < animations[currAnimation].Anchors.Count; ++i)
                    {
                        Rectangle temp = Rectangle.FromLTRB(
                            ImageX + (animations[currAnimation].Frames[i].Left + (animations[currAnimation].Anchors[i].X - 1)),
                            ImageY + (animations[currAnimation].Frames[i].Top + (animations[currAnimation].Anchors[i].Y - 1)),
                            ImageX + (animations[currAnimation].Frames[i].Left + (animations[currAnimation].Anchors[i].X + 1)),
                            ImageY + (animations[currAnimation].Frames[i].Top + (animations[currAnimation].Anchors[i].Y + 1)));

                        D3D.DrawEmptyRect(temp, AnchorColor);
                    }

                }

            }
            D3D.SpriteEnd();
            D3D.DeviceEnd();
            D3D.Present(ImageWindow);

            if (tabControl1.SelectedIndex == 0)
            {
                D3D.Clear(AnimationPreview, BackgroungColor);
                D3D.DeviceBegin();
                D3D.SpriteBegin();
                {
                    if (currAnimation != -1)
                    {
                        animations[currAnimation].Render(PreviewX, PreviewY, 1);
                    }
                }
                D3D.SpriteEnd();
                D3D.DeviceEnd();
                D3D.Present(AnimationPreview);
            }
            if (tabControl1.SelectedIndex == 1)
            {
                D3D.Clear(AnimationPreview2, BackgroungColor);
                D3D.DeviceBegin();
                D3D.SpriteBegin();
                {
                    if (currAnimation != -1)
                    {
                        animations[currAnimation].Render(PreviewX, PreviewY, 1);
                    }
                }
                D3D.SpriteEnd();
                D3D.DeviceEnd();
                D3D.Present(AnimationPreview2);
            }
        }

        private void LoadImagebutton_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                imageID = TM.LoadTexture(dlg.FileName, 0);
                vScrollBar1.Maximum = TM.GetTextureHeight(imageID);
                hScrollBar1.Maximum = TM.GetTextureWidth(imageID);
                currAnimation += 1;
                animations.Add(new Animation());
                animations[currAnimation].TextureID = imageID;
                int index = dlg.FileName.IndexOf("Resource");
                animations[currAnimation].Filepath = dlg.FileName.Substring(index).Replace('\\','/');
                AnimationCollectionListBox.Items.Add(animations[currAnimation].ToString());
                ImagePathTextBox.Text = dlg.FileName;
                ImagePathTextBox2.Text = dlg.FileName;
                ImagePathTextBox3.Text = dlg.FileName;
                PlayButton.Enabled = true;
                PlayTab2Button.Enabled = true;
                NextFrameButton.Enabled = true;
                NextFrameTab2Button.Enabled = true;
                PrevFrameButton.Enabled = true;
                PrevFrameTab2Button.Enabled = true;
                SourceGroup.Enabled = true;
                FramesListBox.Items.Clear();
                EventsListBox.Items.Clear();
                if( mode == Brushes.SOURCE )
                    sourceRectBrushToolStripMenuItem.Checked = true;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            looping = false;
            TM.ShutdownManagedTextureManager();
            D3D.Shutdown();
        }

        private void PlayButton_Click(object sender, EventArgs e)
        {
            animations[currAnimation].Playing = true;
            PauseButton.Enabled = true;
            PauseTab2Button.Enabled = true;
            PlayButton.Enabled = false;
            PlayTab2Button.Enabled = false;
            NextFrameButton.Enabled = false;
            NextFrameTab2Button.Enabled = false;
            PrevFrameButton.Enabled = false;
            PrevFrameTab2Button.Enabled = false;
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            animations[currAnimation].Playing = false;
            PauseButton.Enabled = false;
            PauseTab2Button.Enabled = false;
            PlayButton.Enabled = true;
            PlayTab2Button.Enabled = true;
            NextFrameButton.Enabled = true;
            NextFrameTab2Button.Enabled = true;
            PrevFrameButton.Enabled = true;
            PrevFrameTab2Button.Enabled = true;
        }

        private void NextFrameButton_Click(object sender, EventArgs e)
        {
            animations[currAnimation].CurrFrame++;
        }

        private void PrevFrameButton_Click(object sender, EventArgs e)
        {
            animations[currAnimation].CurrFrame--;
        }

        private void AnimationCollectionListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (AnimationCollectionListBox.SelectedIndex >= animations.Count)
                AnimationCollectionListBox.SelectedIndex = animations.Count - 1;

            currAnimation = AnimationCollectionListBox.SelectedIndex;

            if (currAnimation != -1)
            {
                FrameInfoGroupBox.Enabled = true;
                imageID = animations[currAnimation].TextureID;
                if (imageID != -1)
                {
                    vScrollBar1.Maximum = TM.GetTextureHeight(imageID);
                    hScrollBar1.Maximum = TM.GetTextureWidth(imageID);
                }
            }

            RefreshFrameInfoGroup();
            //Populate();
        }

        private void LoadImageGroupBoxButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                imageID = TM.LoadTexture(dlg.FileName, 0);
                if (imageID != -1)
                {
                    vScrollBar1.Maximum = TM.GetTextureHeight(imageID);
                    hScrollBar1.Maximum = TM.GetTextureWidth(imageID);
                }
                animations[currAnimation].TextureID = imageID;
                animations[currAnimation].Filepath = dlg.FileName;
                ImagePathTextBox.Text = dlg.FileName;
                ImagePathTextBox2.Text = dlg.FileName;
                ImagePathTextBox3.Text = dlg.FileName;
            }
        }

        private void FramesListBox_ControlRemoved(object sender, ControlEventArgs e)
        {
            if (FramesListBox.Items.Count < 1)
            {
                SourceGroup.Enabled = false;
                EventsListBox.Enabled = false;
            }
        }

        private void spriteSheetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                imageID = TM.LoadTexture(dlg.FileName, 0);
                vScrollBar1.Maximum = TM.GetTextureHeight(imageID);
                hScrollBar1.Maximum = TM.GetTextureWidth(imageID);
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDown1.Value >= animations[currAnimation].FrameCount)
                numericUpDown1.Value = animations[currAnimation].FrameCount - 1;

            animations[currAnimation].CurrFrame = (int)numericUpDown1.Value;

            RefreshFrameInfoGroup();
            //Populate();
        }

        private void RemoveAnimationButton_Click(object sender, EventArgs e)
        {
            if (currAnimation != -1)
            {
                AnimationCollectionListBox.Items.Remove(AnimationCollectionListBox.Items[currAnimation]);
                animations.Remove(animations[currAnimation]);
                currAnimation--;
            }
            ResfreshAnimationCollectionBox();
        }

        private void TimePerFrameNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1)
                animations[currAnimation].SetFrameDuration(animations[currAnimation].CurrFrame, (float)TimePerFrameNumeric.Value);
            RefreshFrameInfoGroup();
            //Populate();
        }

        private void AnimationUpButton_Click(object sender, EventArgs e)
        {
            if (AnimationCollectionListBox.SelectedIndex != -1 && currAnimation != -1)
            {
                Animation temp = animations[currAnimation];
                Animation temp2 = animations[currAnimation - 1];
                animations[currAnimation] = temp2;
                animations[currAnimation - 1] = temp;

                AnimationCollectionListBox.SelectedIndex--;
                currAnimation--;


                ResfreshAnimationCollectionBox();
            }
        }

        public void ResfreshAnimationCollectionBox()
        {
            if (currAnimation == -1) return;

            AnimationCollectionListBox.Items.Clear();
            for (int i = 0; i < animations.Count; i++)
                AnimationCollectionListBox.Items.Add(animations[i].ToString());
        }

        private void ImageWindow_MouseClick(object sender, MouseEventArgs e)
        {
            switch (mode)
            {
                case Brushes.ANCHOR:
                    for (int i = 0; i < animations[currAnimation].FrameCount; i++)
                    {
                        if (((-ImageX) + e.X) > animations[currAnimation].Frames[i].Left &&
                            ((-ImageX) + e.X) < animations[currAnimation].Frames[i].Right &&
                            ((-ImageY) + e.Y) > animations[currAnimation].Frames[i].Top &&
                            ((-ImageY) + e.Y) < animations[currAnimation].Frames[i].Bottom)
                        {
                            if (/*anchor == Point.Empty &&*/ currAnimation != -1)
                            {
                                anchor = new Point((((-ImageX) + e.X) - animations[currAnimation].Frames[i].Left),
                                    (((-ImageY) + e.Y) - animations[currAnimation].Frames[i].Top));
                                animations[currAnimation].SetFrameAnchor(i, anchor);
                                anchor = Point.Empty;
                            }
                        }
                    }
                    break;
                case Brushes.COLLSION:
                    for (int i = 0; i < animations[currAnimation].FrameCount; i++)
                    {
                        if (((-ImageX) + e.X) > animations[currAnimation].Frames[i].Left &&
                            ((-ImageX) + e.X) < animations[currAnimation].Frames[i].Right &&
                            ((-ImageY) + e.Y) > animations[currAnimation].Frames[i].Top &&
                            ((-ImageY) + e.Y) < animations[currAnimation].Frames[i].Bottom)
                        {
                            if (collision == Rectangle.Empty && currAnimation != -1)
                            {
                                FrameLeft = animations[currAnimation].Frames[i].Left;
                                FrameTop = animations[currAnimation].Frames[i].Top;
                                collision = Rectangle.FromLTRB((-ImageX) + (e.X - FrameLeft), (-ImageY) + (e.Y - FrameTop), (-ImageX) + (e.X - FrameLeft), (-ImageY) + (e.Y - FrameTop));
                                break;
                            }
                            else if (currAnimation != -1)
                            {
                                Animation_Editor.CollisionEvent temp = new Animation_Editor.CollisionEvent();

                                temp.Rect = collision; temp.Id = "None";
                                animations[currAnimation].Events[i].Add(temp);
                                EventsListBox.Items.Add(temp.ToString());
                                collision = Rectangle.Empty;
                                Populate();
                                break;
                            }
                        }
                    }
                    break;
                case Brushes.SOURCE:
                    if (source == Rectangle.Empty && currAnimation != -1)
                    {
                        source = Rectangle.FromLTRB((-ImageX) + e.X, (-ImageY) + e.Y, (-ImageX) + e.X, (-ImageY) + e.Y);
                        //Populate();
                    }
                    else if (currAnimation != -1)
                    {
                        animations[currAnimation].CreateFrame(source);
                        FramesListBox.Items.Add(source.ToString());
                        source = Rectangle.Empty;
                        //Populate();
                    }
                    break;
            }
        }

        private void ImageWindow_MouseMove(object sender, MouseEventArgs e)
        {
            switch (mode)
            {
                case Brushes.ANCHOR:
                    break;
                case Brushes.COLLSION:
                    if (collision != Rectangle.Empty)
                    {
                        int left = collision.Left;
                        int top = collision.Top;
                        collision = Rectangle.FromLTRB(left, top, (-ImageX) + (e.X - FrameLeft), (-ImageY) + (e.Y - FrameTop));
                        EventRectLeftNumeric.Value = collision.Left;
                        EventRectRightNumeric.Value = collision.Right;
                        EventRectTopNumeric.Value = collision.Top;
                        EventRectBottomNumeric.Value = collision.Bottom;
                    }
                    break;
                case Brushes.SOURCE:
                    if (source != Rectangle.Empty)
                    {
                        int left = source.Left;
                        int top = source.Top;
                        source = Rectangle.FromLTRB(left, top, (-ImageX) + e.X, (-ImageY) + e.Y);
                    }
                    break;
            }
        }

        private void sourceRectBrushToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (animations.Count > 0)
            {
                mode = Brushes.SOURCE;
                sourceRectBrushToolStripMenuItem.Checked = true;
                collisionRectBrushToolStripMenuItem.Checked = false;
                anchorPointBrushToolStripMenuItem.Checked = false;
            }
        }

        private void collisionRectBrushToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (animations.Count > 0)
            {
                mode = Brushes.COLLSION;
                sourceRectBrushToolStripMenuItem.Checked = false;
                collisionRectBrushToolStripMenuItem.Checked = true;
                anchorPointBrushToolStripMenuItem.Checked = false;
            }
        }

        private void anchorPointBrushToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (animations.Count > 0)
            {
                mode = Brushes.ANCHOR;
                sourceRectBrushToolStripMenuItem.Checked = false;
                collisionRectBrushToolStripMenuItem.Checked = false;
                anchorPointBrushToolStripMenuItem.Checked = true;
            }
        }

        private void EventRectEventComboBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                EventRectEventComboBox.Items.Add(EventRectEventComboBox.Text);
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            System.Collections.Specialized.StringCollection temp = new System.Collections.Specialized.StringCollection();

            for (int i = 0; i < EventRectEventComboBox.Items.Count; i++)
                temp.Add(EventRectEventComboBox.Items[i].ToString());

            Properties.Settings.Default.Events = temp;
            Properties.Settings.Default.Save();
        }

        private void SourceRectLeftNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
                animations[currAnimation].Frames[animations[currAnimation].CurrFrame] = Rectangle.FromLTRB((int)SourceRectLeftNumeric.Value, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Top, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Right, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Bottom);
        }

        private void SourceRectRightNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
                animations[currAnimation].Frames[animations[currAnimation].CurrFrame] = Rectangle.FromLTRB(animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Left, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Top, (int)SourceRectRightNumeric.Value, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Bottom);
        }

        private void SourceRectTopNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
                animations[currAnimation].Frames[animations[currAnimation].CurrFrame] = Rectangle.FromLTRB(animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Left, (int)SourceRectTopNumeric.Value, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Right, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Bottom);
        }

        private void SourceRectBottomNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
                animations[currAnimation].Frames[animations[currAnimation].CurrFrame] = Rectangle.FromLTRB(animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Left, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Top, animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Right, (int)SourceRectBottomNumeric.Value);
        }

        private void EventRectEventComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (EventRectEventComboBox.SelectedIndex != -1 && currAnimation != -1)
            {
                CollisionEvent temp = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex];
                temp.Id = EventRectEventComboBox.Items[EventRectEventComboBox.SelectedIndex].ToString();
                animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex] = temp;
            }
        }

        private void XAnchorNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
            {
                anchor.X = (int)XAnchorNumeric.Value;
                Point temp = new Point((int)XAnchorNumeric.Value,animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].Y);
                animations[currAnimation].Anchors[animations[currAnimation].CurrFrame] = temp;
            }
        }

        private void YAnchorNumeric_ValueChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && animations[currAnimation].FrameCount > 0)
            {
                anchor.Y = (int)YAnchorNumeric.Value;
                Point temp = new Point(animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].X, (int)YAnchorNumeric.Value);
                animations[currAnimation].Anchors[animations[currAnimation].CurrFrame] = anchor;
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void FramesListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (currAnimation != -1 && FramesListBox.SelectedIndex != -1)
            {
                animations[currAnimation].CurrFrame = FramesListBox.SelectedIndex;
                RefreshSourceGroup();
                RefreshCollisionGroup();
                RefreshEventsListBox();
            }
        }

        private void EventsListBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            RefreshCollisionGroup();
            //Populate();
        }

        private void newProjectToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            animations.Clear();
            currAnimation = -1;
            imageID = -1;
            AnimationCollectionListBox.Items.Clear();
            FramesListBox.Items.Clear();
            EventsListBox.Items.Clear();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement animationlist = new XElement("animation-list");

                for (int i = 0; i < animations.Count; i++)
                {
                    XElement animation_info = new XElement("animation_info");

                    if (animations[i].AnimationName == null)
                    {
                        MessageBox.Show("One or more animations do not have a name. You must name all animations before saving.",
                            "Cannot Save", MessageBoxButtons.OK);
                        return;
                    }

                    animation_info.Add( new XAttribute( "name", animations[i].AnimationName ) );
                    animation_info.Add( new XAttribute( "type", "anchor" ) );
                    animation_info.Add( new XAttribute( "image_file", animations[i].Filepath ) );
                    animation_info.Add( new XAttribute( "r", 0 ) );
                    animation_info.Add( new XAttribute( "g", 0 ) );
                    animation_info.Add( new XAttribute( "b", 0 ) );
                    animation_info.Add( new XAttribute( "num_anchors", animations[i].FrameCount ) );


                        for(int j = 0; j < animations[i].FrameCount; j++)
                        {
                            XElement anchor = new XElement("anchor");

                            anchor.Add( new XAttribute( "events", animations[i].Events[j].Count ) );

                                XElement rect = new XElement("rect");

                                rect.Add( new XAttribute( "left", animations[i].Frames[j].Left ) );
                                rect.Add( new XAttribute( "right", animations[i].Frames[j].Right ) );
                                rect.Add( new XAttribute( "top", animations[i].Frames[j].Top ) );
                                rect.Add( new XAttribute( "bottom", animations[i].Frames[j].Bottom ) );

                                anchor.Add(rect);

                                XElement point = new XElement("point");

                                point.Add( new XAttribute( "x", animations[i].Anchors[j].X ) );
                                point.Add( new XAttribute( "y", animations[i].Anchors[j].Y ) );

                                anchor.Add(point);

                                XElement duration = new XElement("duration");

                                duration.Add( new XAttribute( "value", animations[i].Tpf[j] ) );

                                anchor.Add(duration);

                                for (int k = 0; k < animations[i].Events[j].Count; k++)
                                {
                                    XElement eventid = new XElement("event");

                                    eventid.Add( new XAttribute( "id", animations[i].Events[j][k].Id ) );
                                    eventid.Add( new XAttribute( "render", animations[i].Events[j][k].IsRender));
                                    eventid.Add( new XAttribute( "left", animations[i].Events[j][k].Rect.Left ) );
                                    eventid.Add( new XAttribute( "top", animations[i].Events[j][k].Rect.Top ) );
                                    eventid.Add( new XAttribute( "right", animations[i].Events[j][k].Rect.Right ) );
                                    eventid.Add( new XAttribute( "bottom", animations[i].Events[j][k].Rect.Bottom ) );

                                    anchor.Add(eventid);
                                }


                            animation_info.Add(anchor);
                        }

                    animationlist.Add(animation_info);
                }

                animationlist.Save(dlg.FileName);
            }
        }

        private void projectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            animations.Clear();
            currAnimation = -1;
            imageID = -1;
            AnimationCollectionListBox.Items.Clear();
            FramesListBox.Items.Clear();
            EventsListBox.Items.Clear();

            OpenFileDialog dlg = new OpenFileDialog();

            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement root = XElement.Load(dlg.FileName);

                IEnumerable<XElement> animationlist = root.Elements();

                foreach (XElement animation_info in animationlist)
                {
                    Animation anime = new Animation();

                    XAttribute image_file = animation_info.Attribute("image_file");
                    if (image_file != null)
                    {
                        anime.Filepath = image_file.Value;
                        anime.TextureID = TM.LoadTexture(image_file.Value,0);
                    }

                    XAttribute name = animation_info.Attribute("name");
                    if (name != null)
                    {
                        anime.AnimationName = name.Value;
                    }

                    IEnumerable<XElement> anchors = animation_info.Elements();
                    foreach (XElement anchor in anchors)
                    {
                        XElement rect = anchor.Element("rect");

                        int nleft = 0, nright = 0, ntop = 0, nbottom = 0;

                        XAttribute left = rect.Attribute("left");
                        if (left != null)
                        {
                            nleft = Convert.ToInt32(left.Value);
                        }
                        XAttribute right = rect.Attribute("right");
                        if (right != null)
                        {
                            nright = Convert.ToInt32(right.Value);
                        }
                        XAttribute top = rect.Attribute("top");
                        if (top != null)
                        {
                            ntop = Convert.ToInt32(top.Value);
                        }
                        XAttribute bottom = rect.Attribute("bottom");
                        if (bottom != null)
                        {
                            nbottom = Convert.ToInt32(bottom.Value);
                        }

                        int i = anime.CreateFrame(nleft,ntop,nright,nbottom);

                        XElement point = anchor.Element("point");

                        Point anchorpoint = Point.Empty; 
                        XAttribute x = point.Attribute("x");
                        if (x != null)
                        {
                            anchorpoint.X = Convert.ToInt32(x.Value);
                        }
                        XAttribute y = point.Attribute("y");
                        if (y != null)
                        {
                            anchorpoint.Y = Convert.ToInt32(y.Value);
                        }
                        anime.SetFrameAnchor(i,anchorpoint);

                        XElement duration = anchor.Element("duration");
                        
                        XAttribute value = duration.Attribute("value");
                        if (value != null)
                        {
                            anime.Tpf[i] = (float)Convert.ToDouble(value.Value);
                        }

                            IEnumerable<XElement> events = duration.ElementsAfterSelf("event");
                            foreach (XElement ev in events)
                            {
                                CollisionEvent newevent = new CollisionEvent();

                                XAttribute id = ev.Attribute("id");
                                if (id != null)
                                {
                                    newevent.Id = id.Value;   
                                }

                                bool isRender = false;
                                XAttribute render = ev.Attribute("render");
                                if (render != null)
                                {
                                    isRender = Convert.ToBoolean(render.Value);
                                }
                                newevent.IsRender = isRender;

                                int xleft = 0, xright = 0, xtop = 0, xbottom = 0;
                                XAttribute cleft = ev.Attribute("left");
                                if (cleft != null)
                                {
                                    xleft = Convert.ToInt32(cleft.Value);
                                }
                                XAttribute cright = ev.Attribute("right");
                                if (cright != null)
                                {
                                    xright = Convert.ToInt32(cright.Value);
                                }
                                XAttribute ctop = ev.Attribute("top");
                                if (ctop != null)
                                {
                                    xtop = Convert.ToInt32(ctop.Value);
                                }
                                XAttribute cbottom = ev.Attribute("bottom");
                                if (cbottom != null)
                                {
                                    xbottom = Convert.ToInt32(cbottom.Value);
                                }
                                newevent.Rect = Rectangle.FromLTRB(xleft,xtop,xright,xbottom);
                                anime.Events[i].Add(newevent);
                            }
                    }
                    imageID = anime.TextureID;
                    if (imageID != -1)
                    {
                        vScrollBar1.Maximum = TM.GetTextureHeight(imageID);
                        hScrollBar1.Maximum = TM.GetTextureWidth(imageID);
                    }
                    animations.Add(anime);
                    currAnimation++;
                    Populate();
                }
                
            }
        }

        public void RefreshFramesListBox()
        {
            if (currAnimation == -1) return;

            int oldselected = FramesListBox.SelectedIndex;

            FramesListBox.Items.Clear();
            for (int i = 0; i < animations[currAnimation].Frames.Count; i++)
                FramesListBox.Items.Add(animations[currAnimation].Frames[i].ToString());

            if ( oldselected >= FramesListBox.Items.Count )
                FramesListBox.SelectedIndex = FramesListBox.Items.Count - 1;
            else
                FramesListBox.SelectedIndex = oldselected;
        }

        public void RefreshEventsListBox()
        {
            if (currAnimation == -1) return;

            int oldselected = EventsListBox.SelectedIndex;

            EventsListBox.Items.Clear();
            if (animations[currAnimation].CurrFrame != -1)
                for (int i = 0; i < animations[currAnimation].Events[animations[currAnimation].CurrFrame].Count; i++)
                    EventsListBox.Items.Add(animations[currAnimation].Events[animations[currAnimation].CurrFrame][i].ToString());
            
            if ( oldselected >= EventsListBox.Items.Count )
                EventsListBox.SelectedIndex = EventsListBox.Items.Count-1;
            else
                EventsListBox.SelectedIndex = oldselected;

        }

        public void RefreshSourceGroup()
        {
            if (currAnimation == -1 || animations[currAnimation].CurrFrame < 0) return;
            SourceRectLeftNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Left;
            SourceRectRightNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Right;
            SourceRectTopNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Top;
            SourceRectBottomNumeric.Value = animations[currAnimation].Frames[animations[currAnimation].CurrFrame].Bottom;
            XAnchorNumeric.Value = animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].X;
            YAnchorNumeric.Value = animations[currAnimation].Anchors[animations[currAnimation].CurrFrame].Y;
        }

        public void RefreshCollisionGroup()
        {
            int oldselected = EventsListBox.SelectedIndex;

            EventRectLeftNumeric.Value = 0;
            EventRectRightNumeric.Value = 0;
            EventRectTopNumeric.Value = 0;
            EventRectBottomNumeric.Value = 0;
            EventRectEventComboBox.Text = "";

            if (oldselected == -1) return;
            EventRectLeftNumeric.Value = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex].Rect.Left;
            EventRectRightNumeric.Value = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex].Rect.Right;
            EventRectTopNumeric.Value = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex].Rect.Top;
            EventRectBottomNumeric.Value = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex].Rect.Bottom;
            EventRectEventComboBox.Text = animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex].Id;

            EventsListBox.SelectedIndex = oldselected;
        }

        public void RefreshFrameInfoGroup()
        {
            ImagePathTextBox.Text = animations[currAnimation].Filepath;
            ImagePathTextBox2.Text = animations[currAnimation].Filepath;
            ImagePathTextBox3.Text = animations[currAnimation].Filepath;
            numericUpDown1.Value = animations[currAnimation].CurrFrame;
            if (animations[currAnimation].CurrFrame != -1)
                TimePerFrameNumeric.Value = (decimal)animations[currAnimation].Tpf[animations[currAnimation].CurrFrame];
            else
                TimePerFrameNumeric.Value = 0;
        }

        public void RefreshAnimationCollectionListBox()
        {
            if (currAnimation == -1) return;

            int oldselected = AnimationCollectionListBox.SelectedIndex;

            AnimationCollectionListBox.Items.Clear();
            for (int i = 0; i < animations.Count; i++)
                AnimationCollectionListBox.Items.Add( animations[i] );

                AnimationCollectionListBox.SelectedIndex = oldselected;
        }

        public void Populate()
        {
            ResfreshAnimationCollectionBox();
            RefreshFramesListBox();
            RefreshSourceGroup();
            RefreshEventsListBox();
            RefreshCollisionGroup();
            RefreshFrameInfoGroup();
        }

        private void RemoveFrameButton_Click(object sender, EventArgs e)
        {
            if (currAnimation == -1) return;

            animations[currAnimation].Frames.Remove(animations[currAnimation].Frames[animations[currAnimation].CurrFrame]);
            animations[currAnimation].Anchors.Remove(animations[currAnimation].Anchors[animations[currAnimation].CurrFrame]);
            animations[currAnimation].Events.Remove(animations[currAnimation].Events[animations[currAnimation].CurrFrame]);
            animations[currAnimation].CurrFrame = animations[currAnimation].CurrFrame - 1;

            Populate();
        }

        private void RemoveCollisionButton_Click(object sender, EventArgs e)
        {
            if (currAnimation == -1) return;

            animations[currAnimation].Events[animations[currAnimation].CurrFrame].Remove(animations[currAnimation].Events[animations[currAnimation].CurrFrame][EventsListBox.SelectedIndex]);

            Populate();
        }

        private void sourceRectColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                SourceColor = dlg.Color;
            }
        }

        private void collisionRectColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                EventColor = dlg.Color;
            }
        }

        private void anchorPointColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                AnchorColor = dlg.Color;
            }
        }

        private void AnimationNameTextBox_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void AnimationNameTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == '\r')
            {
                if (currAnimation == -1) return;
                animations[currAnimation].AnimationName = AnimationNameTextBox.Text;
                RefreshAnimationCollectionListBox();
            }
        }

        private void backgroundColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                BackgroungColor = dlg.Color;
            }
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ImageY = -e.NewValue;
        }

        private void hScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            ImageX = -e.NewValue;
        }

        private void isRenderEventChectbox_CheckedChanged(object sender, EventArgs e)
        {
            if (currAnimation == -1) return;

            int x = animations[currAnimation].CurrFrame;
            int i = EventsListBox.SelectedIndex;

            if (x < 0 || i == -1) return;

            CollisionEvent temp = animations[currAnimation].Events[x][i];
            
            temp.IsRender = isRenderEventChectbox.Checked;
            animations[currAnimation].Events[x][i] = temp;
        }

        private void DuplicateFrameButton_Click(object sender, EventArgs e)
        {
            if (currAnimation == -1 || FramesListBox.SelectedIndex == -1) return;

            animations[currAnimation].Frames.Add(animations[currAnimation].Frames[FramesListBox.SelectedIndex]);
            animations[currAnimation].Tpf.Add(animations[currAnimation].Tpf[FramesListBox.SelectedIndex]);
            animations[currAnimation].Anchors.Add(animations[currAnimation].Anchors[FramesListBox.SelectedIndex]);
            animations[currAnimation].Events.Add(animations[currAnimation].Events[FramesListBox.SelectedIndex]);
        }
    }
}
