using System;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SGP;

namespace Animation_Editor
{
    public struct CollisionEvent
    {
        string id;

        public string Id
        {
            get { return id; }
            set { id = value; }
        }
        Rectangle rect;

        public Rectangle Rect
        {
            get { return rect; }
            set { rect = value; }
        }

        bool isrender;

        public bool IsRender
        {
            get { return isrender; }
            set { isrender = value; }
        }
        
        public override string ToString()
        {
            string value;
            value = "Event: " + id + "Width: " + (rect.Right-rect.Left) + " Height: " + (rect.Bottom-rect.Top);
            return value;
        }
    };

    class Animation
    {
        ManagedTextureManager TM = ManagedTextureManager.Instance;
        string filepath;
        string animationname;
        char a;
        char r;
        char g;
        char b;
        int textureID = -1;
        int currFrame = 0;
        float elapsed = 0;
        bool playing;
        List<Rectangle> frames = new List<Rectangle>();
        List<float> tpf = new List<float>();
        List<Point> anchors = new List<Point>();
        List<List<CollisionEvent>> events = new List<List<CollisionEvent>>();

        public string AnimationName
        {
            get { return animationname; }
            set { animationname = value; }
        }

        public List<float> Tpf
        {
            get { return tpf; }
            set { tpf = value; }
        }

        

        internal List<List<CollisionEvent>> Events
        {
            get { return events; }
        }

        public List<Point> Anchors
        {
            get { return anchors; }
        }

        public List<Rectangle> Frames
        {
            get { return frames; }
        }

        public int FrameCount
        {
            get { return frames.Count; }
        }

        public bool Playing
        {
            get { return playing; }
            set { playing = value; }
        }

        public string Filepath
        {
            get { return filepath; }
            set { filepath = value; }
        }

        public int CurrFrame
        {
            get { return currFrame; }
            set { currFrame = value; }
        }

        public int TextureID
        {
            get { return textureID; }
            set { textureID = value; }
        }

        public float GetFrameDuration(decimal frame)
        {
            if (frame >= frames.Count)
                return -1.0f;

            return tpf[(int)frame];
        }

        public void SetFrameDuration(decimal frame, float value)
        {
            if (frame >= frames.Count || frame == -1)
                return;

            tpf[(int)frame] = value;
        }

        public void SetFrameAnchor(int frame,Point anchor)
        {
            if (frame < 0 || frame > FrameCount)
                return;

            anchors[frame] = anchor;
        }

        public int CreateFrame(int left, int top, int right, int bottom)
        {
            Rectangle rect = Rectangle.FromLTRB(left, top, right, bottom);

            frames.Add(rect);
            tpf.Add(0.12f);
            anchors.Add(new Point(0,0));
            events.Add(new List<CollisionEvent>());

            return frames.Count - 1;
        }

        public int CreateFrame(Rectangle source)
        {
            Rectangle rect = Rectangle.FromLTRB(source.Left, source.Top, source.Right, source.Bottom);

            frames.Add(rect);
            tpf.Add(0.12f);
            anchors.Add(new Point(0, 0));
            events.Add(new List<CollisionEvent>());

            return frames.Count - 1;
        }

        public void Update(float dt)
        {
            elapsed += dt;


            if (currFrame < 0)
                currFrame = frames.Count - 1;
            if (currFrame >= frames.Count)
                currFrame = 0;

            if (!playing || currFrame == -1)
                return;

            if (elapsed > tpf[currFrame])
            {
                currFrame++;
                elapsed = 0;
            }

            if (currFrame < 0)
                currFrame = frames.Count - 1;
            if (currFrame >= frames.Count)
                currFrame = 0;
        }

        public void Render(int x, int y, float scale)
        {
            if (textureID != -1 && frames.Count > 0)
                TM.Draw(textureID, x - anchors[currFrame].X, y - anchors[currFrame].Y, scale, scale, frames[currFrame], 0, 0, 0, Color.White);
        }

        public override string ToString()
        {
            float time = 0;
            for (int i = 0; i < tpf.Count; i++)
                time += tpf[i];
            string value;
            value = animationname + " Frames: " + frames.Count + " Total Time: " + time;
            return value;
        }
    }
}
