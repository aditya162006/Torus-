#include <stdio.h>
#include <math.h>


#define PI 3.141592
int main()
{
    int height = 40;
    int width = 80;
    char screen_buffer[width * height];
    double depth_buffer[width * height];
    float rud = 0;
    float rlr = 0;
    float R = 12;  
    float r = 4;
    int D = 40;    
    int scale_factor = 35;
    printf("\x1b[?25l");

    while (1==1)
    {
            for(int i = 0; i < width * height; i++)
        {
            screen_buffer[i] = ' ';
            depth_buffer[i] = -10000;
        }
        
        rud = rud + 0.03;
        rlr = rlr + 0.03;

            for(float theta = 0; theta < 2*PI; theta = theta + 0.04 )
            {
                for(float phi = 0; phi < 2*PI; phi = phi + 0.04)
                    {
                        float Xc = R*cos(theta); 
                        float Yc = R*sin(theta); 
                        float Zc = 0;
                        
                        float Xt = r*cos(phi)*cos(theta);
                        float Yt = r*cos(phi)*sin(theta);
                        float Zt = r*sin(phi);

                        float X = Xc + Xt;
                        float Y = Yc + Yt;
                        float Z = Zc + Zt;

                        float ry = Y*cos(rud) - Z*sin(rud);
                        float rz = Y*sin(rud) + Z*cos(rud);
                        float rx = X;
                        

                        float final_rx = rx*cos(rlr) - ry*sin(rlr);
                        float final_ry = rx*sin(rlr) + ry*cos(rlr);
                        float final_rz = rz;
                        
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
                                int index = screen_x + screen_y * width;

                                float depth_value = 1.0/(final_rz + D);
                               if (depth_value > depth_buffer[index])
                                    {
                                        depth_buffer[index] = depth_value;
        
                                        
                                        float nx = cos(phi)*cos(theta);
                                        float ny = cos(phi)*sin(theta);
                                        float nz = sin(phi);

                                        float rny = ny*cos(rud) - nz*sin(rud);
                                        float rnz = ny*sin(rud) + nz*cos(rud);
                                        float rnx = nx;

                                        float final_rnx = rnx*cos(rlr) - rny*sin(rlr);
                                        float final_rny = rnx*sin(rlr) + rny*cos(rlr);
                                        float final_rnz = rnz;

                                        float lx = 0;
                                        float ly = 0.7;
                                        float lz = -0.7;


                                        float dot = final_rnx * lx + final_rny * ly + final_rnz * lz;

                                        float brightness = (dot + 1) * 0.5;

                                        if (brightness < 0)
                                        {
                                            brightness = 0;
                                        }
                                        if (brightness > 1)
                                        {
                                            brightness = 1;
                                        }
                                        char gradient[] = " .,-~:;=!*#$@";

                                        int idx = (int)(brightness * 12);
                                        screen_buffer[index] = gradient[idx];

                                    }

                            }
                    

                    }
            }   
                                    printf("\x1b[H"); 
                                    
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

    
                                        printf("\x1b[?25h");
    return 0;

}
