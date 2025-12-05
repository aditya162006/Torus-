#include <stdio.h>
#include <math.h>


#define PI 3.141592
int main()
{
    int height = 40;
    int width = 80;
    char screen_buffer[width * height];
    double depth_buffer[width * height];
    float rud = 0; //for up down rotation
    float rlr = 0; // for left right rotation
    float R = 12;  
    float r = 4;
    int D = 40;    
    int scale_factor = 35;
    printf("\x1b[?25l"); // hide cursor

    while (1==1)
    {
        // 1. Clear screen and depth buffers
            for(int i = 0; i < width * height; i++)
        {

             /*Clearing screen_buffer means:
                “Erase all characters from the previous frame.”
            Clearing depth_buffer means:
                “Reset the stored distances so new points can be tested for which is in front.”*/
            screen_buffer[i] = ' ';
            depth_buffer[i] = -10000;
        }

    // 2. Update rotation angles
        rud = rud + 0.03;
        rlr = rlr + 0.03;
    // 3. Generate torus points in 3D
            for(float theta = 0; theta < 2*PI; theta = theta + 0.04 )
            {
                for(float phi = 0; phi < 2*PI; phi = phi + 0.04)
                    {
                        //Parametric equation of the bigger circle
                        float Xc = R*cos(theta); 
                        float Yc = R*sin(theta); 
                        float Zc = 0;
                        
                        //Parametric equation of the smaller circle
                        float Xt = r*cos(phi)*cos(theta);
                        float Yt = r*cos(phi)*sin(theta);
                        float Zt = r*sin(phi);

                        //parametric equation of a point on the surface of torus.
                        float X = Xc + Xt;
                        float Y = Yc + Yt;
                        float Z = Zc + Zt;


                        //rotation about x axis or y-z plane
                        
                        float ry = Y*cos(rud) - Z*sin(rud);
                        float rz = Y*sin(rud) + Z*cos(rud);
                        float rx = X;
                        //rotation about z axis or x-z plane

                        float final_rx = rx*cos(rlr) - ry*sin(rlr);
                        float final_ry = rx*sin(rlr) + ry*cos(rlr);
                        float final_rz = rz;


                        // 4. Project 3D points onto 2D screen buffer
                        
                        float persx = final_rx/(final_rz + D);
                        float persy = final_ry/(final_rz + D);

                        float px_scaled = persx*scale_factor;
                        float py_scaled = persy*scale_factor;

                        float screen_center_x = width / 2;
                        float screen_center_y = height / 2;

                        int screen_x = screen_center_x + px_scaled;
                        int screen_y = screen_center_y - py_scaled;

                        

                        if (screen_x >= 0 && screen_x < width && screen_y >= 0 && screen_y < height)
                            {
                                // then compute index and depth test
                                int index = screen_x + screen_y * width;

                                float depth_value = 1.0/(final_rz + D);
                               if (depth_value > depth_buffer[index])
                                    {
                                        depth_buffer[index] = depth_value;
        
                                        
                                        float nx = cos(phi)*cos(theta);
                                        float ny = cos(phi)*sin(theta);
                                        float nz = sin(phi);

                                        //rotation about x axis or y-z plane
                        
                                        float rny = ny*cos(rud) - nz*sin(rud);
                                        float rnz = ny*sin(rud) + nz*cos(rud);
                                        float rnx = nx;
                                        //rotation about z axis or x-z plane

                                        float final_rnx = rnx*cos(rlr) - rny*sin(rlr);
                                        float final_rny = rnx*sin(rlr) + rny*cos(rlr);
                                        float final_rnz = rnz;

                                        float lx = 0;
                                        float ly = 0.7;
                                        float lz = -0.7;


                                        // Dot product for brightness (use rotated normal!)
                                        float dot = final_rnx * lx + final_rny * ly + final_rnz * lz;

                                        // Normalize brightness to 0–1
                                        float brightness = (dot + 1) * 0.5;

                                        // Clamp brightness
                                        if (brightness < 0)
                                        {
                                            brightness = 0;
                                        }
                                        if (brightness > 1)
                                        {
                                            brightness = 1;
                                        }

                                        // ASCII brightness characters
                                        char gradient[] = " .,-~:;=!*#$@";

                                        // Pick character
                                        int idx = (int)(brightness * 12); // gradient has 13 chars
                                        screen_buffer[index] = gradient[idx];

                                    }

                            }
                    

                    }
            }   

                                // Print the screen buffer to the terminal
                                // Move cursor to top-left of terminal
                                    //printf("\x1b[2J");  // Clear entire screen
                                    printf("\x1b[H");   // Move cursor to top-left
                                    

                                


                                // Print each row
                                    for (int y = 0; y < height; y++)
                                        {
                                            for (int x = 0; x < width; x++)
                                                {
                                                    putchar(screen_buffer[x + y * width]);
                                                }
                                                    putchar('\n');
                                        }
                                        fflush(stdout);
                                        

                                        

                                

    }

    
                                        printf("\x1b[?25h"); // show cursor
   
}