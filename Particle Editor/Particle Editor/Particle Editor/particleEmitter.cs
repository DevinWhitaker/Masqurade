using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using SGP;
using System.Drawing;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;


namespace Particle_Editor
{

    public class tColorStop
    {
        public Microsoft.DirectX.Direct3D.ColorValue color;
        public float coef;

        public tColorStop(int foo)
        {
            color = new Microsoft.DirectX.Direct3D.ColorValue(1.0f, 1.0f, 1.0f, 1.0f);
            coef = 0.0f;
        }

        static public bool operator ==(tColorStop t, tColorStop s)
        {
            if (t.color.Alpha == s.color.Alpha &&
                t.color.Red == s.color.Red &&
                t.color.Green == s.color.Green &&
                t.color.Blue == s.color.Blue &&
                t.coef == s.coef)
                return true;
            else
                return false;
        }

        static public bool operator !=(tColorStop t, tColorStop s)
        {
            if (t.color.Alpha != s.color.Alpha ||
                t.color.Red != s.color.Red ||
                t.color.Green != s.color.Green ||
                t.color.Blue != s.color.Blue ||
                t.coef != s.coef)
                return true;
            else
                return false;
        }


    }

    public class tSizeStop
    {
        public Microsoft.DirectX.Vector2 scale;
        public float coef;

        public tSizeStop(int foo)
        {
            scale = new Microsoft.DirectX.Vector2(1.0f, 1.0f);
            coef = 0.0f;
        }

        static public bool operator ==(tSizeStop t, tSizeStop s)
        {
            if (s.scale.X == t.scale.X &&
                s.scale.Y == t.scale.Y &&
                s.coef == t.coef)
                return true;
            else
                return false;
        }

        static public bool operator !=(tSizeStop t, tSizeStop s)
        {
            if (s.scale.X == t.scale.X &&
                s.scale.Y == t.scale.Y &&
                s.coef == t.coef)
                return false;
            else
                return true;
        }
    }

    public class tVelStop
    {
        public Microsoft.DirectX.Vector2 vel;
        public float coef;

        public tVelStop(int foo)
        {
            vel = Microsoft.DirectX.Vector2.Empty;
            coef = 0.0f;
        }

        static public bool operator ==(tVelStop t, tVelStop s)
        {
            if (s.vel.X == t.vel.X &&
                s.vel.Y == t.vel.Y &&
                s.coef == t.coef)
                return true;
            else
                return false;
        }

        static public bool operator !=(tVelStop t, tVelStop s)
        {
            if (s.vel.X == t.vel.X &&
                s.vel.Y == t.vel.Y &&
                s.coef == t.coef)
                return false;
            else
                return true;
        }
    }

    public class tParticle
    {
        public Microsoft.DirectX.Vector2 pos;
        public Microsoft.DirectX.Vector2 vel;
        public Microsoft.DirectX.Direct3D.ColorValue color;
        public float age;
        public float width;
        public float height;
        public int curColor;
        public int curSize;
        public int curVel;

        public List<int> colorIndices;
        public List<int> sizeIndices;
        public List<int> velIndices;

        public tParticle(int foo)
        {
            pos = Microsoft.DirectX.Vector2.Empty;
            vel = Microsoft.DirectX.Vector2.Empty;
            color = new Microsoft.DirectX.Direct3D.ColorValue(1.0f, 1.0f, 1.0f, 1.0f);
            age  = 0.0f;
            width = 1.0f;
            height = 1.0f;
            curColor = 0;
            curSize = 0;
            curVel = 0;

            colorIndices = new List<int>();
            sizeIndices = new List<int>();
            velIndices = new List<int>();
        }
    }

    public class particleEmitter
    {
        enum eShapes { RECTANGLE, CIRCLE, POINT };

        

        public List<tParticle> particles = new List<tParticle>();

        public List<tColorStop> uColors = new List<tColorStop>();
        public List<tSizeStop> uSizes = new List<tSizeStop>();
        public List<tVelStop> uVels = new List<tVelStop>();

        public string filename = "";
        public string name = "";
        public string filepath = "";
        public int imageID = -1;
        public Microsoft.DirectX.Vector2 pos = Microsoft.DirectX.Vector2.Empty;
        public Vector2 startPos = new Vector2();
        public float age = 0.0f;
        public float eDuration = 10.0f;
        public float pDuration = 10.0f;
        public bool isLooping = true;
        public bool isBurst = false;
        public int shape = (int)eShapes.POINT;
        public Microsoft.DirectX.Vector2 force = Microsoft.DirectX.Vector2.Empty;
        public float particleScalar = 1.00f;
        public System.Drawing.Rectangle pRect = System.Drawing.Rectangle.Empty;
        public float radius = 0.0f;
        public bool isBirth = true;
        public int sourceBlendMode = 5;
        public int destBlendMode = 14;
        public int partsActive = 0;
        public int deadCutoff = 0;
        Random rand = new Random(System.DateTime.Now.Millisecond);
        

        //public bool isBurst
        //{
        //    get { return isBurst; }
        //    set { isBurst = value; }
        //}

        public static void swap(tColorStop lhs, tColorStop rhs)
        {
            tColorStop temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        public static void swap(tSizeStop lhs, tSizeStop rhs)
        {
            tSizeStop temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        public static void swap(tVelStop lhs, tVelStop rhs)
        {
            tVelStop temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        public static void swap(int lhs, int rhs)
        {
            int temp;
            temp = lhs;
            lhs = rhs;
            rhs = temp;
        }

        public void RenderParticles(int ID = -2)
        {

            if (age >= eDuration && !isLooping)
                return;

            int id = imageID;
            if (ID != -2 )
                id = ID;

            if (id != -1)
            {
                for (int i = deadCutoff; i < partsActive; ++i)
                {
                    //particles[i].color = new ColorValue(1.0f,1.0f,1.0f,1.0f);
                    ManagedTextureManager.Instance.Draw(id,
                        (int)(particles[i].pos.X - ManagedTextureManager.Instance.GetTextureWidth(imageID) / 2.0f * particles[i].width),
                        (int)(particles[i].pos.Y - ManagedTextureManager.Instance.GetTextureWidth(imageID) / 2.0f * particles[i].height),
                        particles[i].width,
                        particles[i].height,
                        Rectangle.Empty, 0, 0, 0, particles[i].color.ToArgb());
                }
            }
        }

        public void UpdateParticles(float fElapsed, float anchorToTargetX = -1, float anchorToTargetY = -1)
        {
            
            if (eDuration >= 0.0000f)
                age += fElapsed;

            if (age >= eDuration && ! isLooping)
                return;

            if (anchorToTargetX != -1 && anchorToTargetY != -1)
            {

            }

            if (shape == (int)eShapes.CIRCLE || shape == (int)eShapes.RECTANGLE)
            {
                pRect.X = (int)(pos.X - pRect.Width / 2.0f);
                pRect.Y = (int)(pos.Y - pRect.Height / 2.0f);
            }

            if (isBirth && !isBurst)
            {
                partsActive = (int)(age/pDuration*particles.Count());
                if (age >= pDuration)
                {
                    isBirth = false;
                    partsActive = (int)particles.Count();
                }
            }

            //for (int i = 0; i < particles.Count(); ++i)
            for (int i = deadCutoff; i < partsActive; ++i)
            {
                float width = particles[i].width;
                float height = particles[i].height;
                if (width < 0.0f || height < 0.0f)
                    width += 0.000f;
                if (width > 2.0f || height > 2.0f)
                    width += 0.000f;    

                float sizeLimit = 5.0f;
                if (particles[i].width >= sizeLimit || particles[i].width <= -sizeLimit ||
                    particles[i].height >= sizeLimit || particles[i].height <= -sizeLimit)
                {
                    sizeLimit = sizeLimit * 2;
                }

                if (particles[i].age >= pDuration || particles[i].age == 0.0f)
                {
                    if (isLooping)
                        SpawnParticle(i);
                    else if (particles[i].age >= pDuration)
                        ++deadCutoff;
                }

                if (particles[i].curColor < particles[i].colorIndices.Count() - 1)
                {
                    float diff = 0.0f;
                    int curColorStop = particles[i].colorIndices[particles[i].curColor];
                    int nextColorStop = particles[i].colorIndices[particles[i].curColor + 1];

                    diff = uColors[nextColorStop].coef - uColors[curColorStop].coef;

                    float curPos = 0.0f;
                    curPos = particles[i].age / pDuration - uColors[curColorStop].coef;

                    float coef = (diff != 0) ? curPos / diff : 1.0f;

                    Microsoft.DirectX.Direct3D.ColorValue curColor = new Microsoft.DirectX.Direct3D.ColorValue();
                    Microsoft.DirectX.Direct3D.ColorValue nextColor = new Microsoft.DirectX.Direct3D.ColorValue();

                    curColor = uColors[curColorStop].color;
                    nextColor = uColors[nextColorStop].color;

                    particles[i].color.Alpha = curColor.Alpha + (nextColor.Alpha - curColor.Alpha) * coef;
                    particles[i].color.Red = curColor.Red + (nextColor.Red - curColor.Red) * coef;
                    particles[i].color.Green = curColor.Green + (nextColor.Green - curColor.Green) * coef;
                    particles[i].color.Blue = curColor.Blue + (nextColor.Blue - curColor.Blue) * coef;

                    if (particles[i].age / pDuration >= uColors[nextColorStop].coef)
                        ++particles[i].curColor;

                }
               
                if (particles[i].curSize < particles[i].sizeIndices.Count() - 1)
                {
                    float diff = 0.0f;
                    int curSizeStop = particles[i].sizeIndices[particles[i].curSize];
                    int nextSizeStop = particles[i].sizeIndices[particles[i].curSize + 1];

                    diff = uSizes[nextSizeStop].coef - uSizes[curSizeStop].coef;

                    float curPos = 0.0f;
                    curPos = particles[i].age / pDuration - uSizes[curSizeStop].coef;

                    float coef = (diff != 0) ? curPos / diff : 1.0f;

                    Vector2 curSize = new Vector2();
                    curSize.X = uSizes[curSizeStop].scale.X;
                    curSize.Y = uSizes[curSizeStop].scale.Y;

                    Vector2 nextSize = new Vector2();
                    nextSize.X = uSizes[nextSizeStop].scale.X;
                    nextSize.Y = uSizes[nextSizeStop].scale.Y;

                    particles[i].width = (curSize.X + (nextSize.X - curSize.X) * coef) * particleScalar;
                    particles[i].height = (curSize.Y + (nextSize.Y - curSize.Y) * coef) * particleScalar;
                                     

                    if (particles[i].age / pDuration >= uSizes[nextSizeStop].coef)
                        ++particles[i].curSize;
                }

                           

                if (particles[i].curVel < particles[i].velIndices.Count() - 1)
                {
                    float diff = 0.0f;
                    int curVelStop = particles[i].velIndices[particles[i].curVel];
                    int nextVelStop = particles[i].velIndices[particles[i].curVel + 1];

                    diff = uVels[nextVelStop].coef - uVels[curVelStop].coef;

                    float curPos = 0.0f;
                    curPos = particles[i].age / pDuration - uVels[curVelStop].coef;

                    float coef = (diff != 0) ? curPos / diff : 1.0f;

                    Vector2 curVel = new Vector2();
                    curVel.X = uVels[curVelStop].vel.X;
                    curVel.Y = uVels[curVelStop].vel.Y;

                    Vector2 nextVel = new Vector2();
                    nextVel.X = uVels[nextVelStop].vel.X;
                    nextVel.Y = uVels[nextVelStop].vel.Y;

                    particles[i].vel.X = curVel.X + (nextVel.X - curVel.X) * coef;
                    particles[i].vel.Y = curVel.Y + (nextVel.Y - curVel.Y) * coef;

                    if (particles[i].age / pDuration >= uVels[nextVelStop].coef)
                        ++particles[i].curVel;
                }

                // FIX THIS !!!!!!!
                // !!!!!!!!!!!!!!!                                           THIS NEEDS FIXING....
                particles[i].pos.X += (particles[i].vel.X + force.X) * fElapsed;// *100;
                particles[i].pos.Y += (particles[i].vel.Y + force.Y) * fElapsed;// *100;

                particles[i].age += fElapsed;

                   
            }

            
        }


        public void SpawnParticle(int i)
        {
            if (shape == (int)eShapes.POINT)
            {
                particles[i].pos = pos;
                if (particles[i].sizeIndices.Count > 0)
                {
                    particles[i].width = 1.0f;
                    particles[i].height = 1.0f;
                    //particles[i].width = uSizes[particles[i].sizeIndices[0]].scale.X;
                    //particles[i].height = uSizes[particles[i].sizeIndices[0]].scale.Y;
                }
            }
            else
            {
                RandParticleInRect(i);
            }

            particles[i].curColor = 0;
            particles[i].curSize = 0;
            particles[i].curVel = 0;
            particles[i].age = 0;
        }

        public void RandParticleInRect(int i)
        {
            
            for (; ; )
            {
                particles[i].pos.X = rand.Next(pRect.Location.X, pRect.Location.X + pRect.Width);
                particles[i].pos.Y = rand.Next(pRect.Location.Y, pRect.Location.Y + pRect.Height);

                if (shape == (int)eShapes.CIRCLE)
                {
                    float distance = (float)Math.Sqrt(Math.Pow((particles[i].pos.X - pos.X), 2) + (Math.Pow((particles[i].pos.Y - pos.Y), 2)));
                    if (distance < radius || radius <= 10)
                        break;
                    else
                        continue;
                }

                if (shape == (int)eShapes.POINT || shape == (int)eShapes.RECTANGLE ||
                    radius <= 0 || pRect == Rectangle.Empty )
                    break;
            }
        }

        public void reset()
        {
            age = 0.0f;
            isBirth = true;
            pos = startPos;
            pRect.X = (int)startPos.X-pRect.Location.X/2;
            pRect.Y = (int)startPos.Y - pRect.Location.Y / 2;
            deadCutoff = 0;
            if (isBurst)
                partsActive = particles.Count();
            else
                partsActive = 0;

            foreach (tParticle p in particles)
            {
                p.age = 0.0f;
                p.pos = pos;
                
                p.curColor = 0;
                p.curSize = 0;
                p.curVel = 0;

                if(p.colorIndices.Count() > 0)
                    p.color = uColors[p.colorIndices[p.curColor]].color;
                else
                    p.color = new ColorValue(1.0f,1.0f,1.0f,1.0f);
                
                if(p.sizeIndices.Count() > 0)
                {
                    p.width = uSizes[p.sizeIndices[p.curSize]].scale.X;
                    p.height = uSizes[p.sizeIndices[p.curSize]].scale.Y;
                }
                else
                {
                    p.width = 1.0f;
                    p.height = 1.0f;
                }   

                if(p.velIndices.Count() > 0)
                    p.vel = uVels[p.velIndices[p.curVel]].vel;
                else
                    p.vel = new Vector2(0.0f, 0.0f);

            }
        }

        public int getColorIndex(tColorStop t)
        {
            for (int i = 0; i < uColors.Count(); ++i)
            {
                if (uColors[i] == t)
                    return i;
            }
            return -1;
        }

        public int getSizeIndex(tSizeStop t)
        {
            for (int i = 0; i < uSizes.Count(); ++i)
            {
                if (uSizes[i] == t)
                    return i;
            }
            return -1;
        }

        public int getVelocityIndex(tVelStop t)
        {
            for (int i = 0; i < uVels.Count(); ++i)
            {
                if (uVels[i] == t)
                    return i;
            }
            return -1;
        }

        public void sortStops()
        {
            // bubble sort the colors
            for (int i = 0; i < uColors.Count(); ++i)
            {
                for (int j = 0; j < uColors.Count(); ++j)
                {
                    // swap the colors
                    if (uColors[i].coef > uColors[j].coef)
                    {

                        // check any particles
                        for (int x = 0; x < particles.Count(); ++x)
                        {
                            // to see if they match the indicies of the stops going to be swapped
                            for (int y = 0; y < particles[x].colorIndices.Count(); ++y)
                            {
                                if (particles[x].colorIndices[y] == i)
                                    particles[x].colorIndices[y] = j;
                                else if (particles[x].colorIndices[y] == j)
                                    particles[x].colorIndices[y] = i;
                            }
                        }
                        swap(uColors[i], uColors[j]);


                    }
                }
            }
            int foo = 0;
            // same steps for size stops
            for (int i = 0; i < uSizes.Count(); ++i)
            {
                for (int j = 0; j < uSizes.Count(); ++j)
                {
                    // swap the stops
                    if (uSizes[i].coef > uSizes[j].coef)
                    {

                        // check any particles
                        for (int x = 0; x < particles.Count(); ++x)
                        {
                            // to see if they match the indicies of the stops going to be swapped
                            for (int y = 0; y < particles[x].sizeIndices.Count(); ++y)
                            {
                                if (particles[x].sizeIndices[y] == i)
                                    particles[x].sizeIndices[y] = j;
                                else if (particles[x].sizeIndices[y] == j)
                                    particles[x].sizeIndices[y] = i;
                            }
                        }
                        swap(uSizes[i], uSizes[j]);


                    }
                }
            }

            // same steps for velocity stops
            for (int i = 0; i < uVels.Count(); ++i)
            {
                for (int j = 0; j < uVels.Count(); ++j)
                {
                    // swap the stops
                    if (uVels[i].coef > uVels[j].coef)
                    {

                        // check any particles
                        for (int x = 0; x < particles.Count(); ++x)
                        {
                            // to see if they match the indicies of the stops going to be swapped
                            for (int y = 0; y < particles[x].velIndices.Count(); ++y)
                            {
                                if (particles[x].velIndices[y] == i)
                                    particles[x].velIndices[y] = j;
                                else if (particles[x].velIndices[y] == j)
                                    particles[x].velIndices[y] = i;
                            }
                        }
                        swap(uVels[i], uVels[j]);


                    }
                }
            }

            //// loop through the particles
            //for (int i = 0; i < particles.Count(); ++i)
            //{
            //    // sort the color indices based on the stop's coef
            //    for (int x = 0; x < particles[i].colorIndices.Count(); ++x)
            //    {
            //        for (int y = 0; y < particles[i].colorIndices.Count(); ++y)
            //        {
            //            if (uColors[particles[i].colorIndices[x]].coef <
            //                uColors[particles[i].colorIndices[y]].coef)
            //                swap(particles[i].colorIndices[x], particles[i].colorIndices[y]);
            //        }
            //    }


            //    // sort the size indices based on the stop's coef
            //    for (int x = 0; x < particles[i].sizeIndices.Count(); ++x)
            //    {
            //        for (int y = 0; y < particles[i].sizeIndices.Count(); ++y)
            //        {
            //            if (uSizes[particles[i].sizeIndices[x]].coef <
            //                uSizes[particles[i].sizeIndices[y]].coef)
            //                swap(particles[i].sizeIndices[x], particles[i].sizeIndices[y]);
            //        }
            //    }

            //    // sort the velocity indices based on the stop's coef
            //    for (int x = 0; x < particles[i].velIndices.Count(); ++x)
            //    {
            //        for (int y = 0; y < particles[i].velIndices.Count(); ++y)
            //        {
            //            if (uVels[particles[i].velIndices[x]].coef <
            //                uVels[particles[i].velIndices[y]].coef)
            //                swap(particles[i].velIndices[x], particles[i].velIndices[y]);
            //        }
            //    }


            //}


        }
        
    }
}
