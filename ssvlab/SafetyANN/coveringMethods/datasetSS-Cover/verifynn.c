#include <verifynn.h>
#include <stdlib.h>


float wfc1[125]={ 0.207958,  0.044154,  -4.232451,  0.745003,  -1.340141,  -1.486082,  -1.869501,  -1.486536,  -0.624669,  1.210342,  -4.070879,  -0.600616,  -5.623879,  -3.807880,  1.362974,  1.334723,  -2.295741,  3.483194,  -4.446033,  0.078016,  4.587534,  -0.206197,  0.977980,  1.286488,  0.772414,
-1.486082,  -1.869501,  -1.486536,  -0.624669,  1.210342,  -4.070879,  -0.600616,  -5.623879,  -3.807880,  1.362974,  1.334723,  -2.295741,  3.483194,  -4.446033,  0.078016,  4.587534,  -0.206197,  0.977980,  1.286488,  0.772414,  -0.095517,  -0.237044,  1.996482,  2.389851,  -0.082591,
-4.070879,  -0.600616,  -5.623879,  -3.807880,  1.362974,  1.334723,  -2.295741,  3.483194,  -4.446033,  0.078016,  4.587534,  -0.206197,  0.977980,  1.286488,  0.772414,  -0.095517,  -0.237044,  1.996482,  2.389851,  -0.082591,  2.035967,  -1.636084,  -2.794725,  -1.262111,  6.436423,
1.334723,  -2.295741,  3.483194,  -4.446033,  0.078016,  4.587534,  -0.206197,  0.977980,  1.286488,  0.772414,  -0.095517,  -0.237044,  1.996482,  2.389851,  -0.082591,  2.035967,  -1.636084,  -2.794725,  -1.262111,  6.436423,  -0.922335,  -2.358342,  -0.142077,  -4.891456,  -2.749024,
4.587534,  -0.206197,  0.977980,  1.286488,  0.772414,  -0.095517,  -0.237044,  1.996482,  2.389851,  -0.082591,  2.035967,  -1.636084,  -2.794725,  -1.262111,  6.436423,  -0.922335,  -2.358342,  -0.142077,  -4.891456,  -2.749024,  0.042693,  0.455576,  0.108683,  0.724179,  0.679236 };

float bfc1[5]={ -0.095517, 2.035967, -0.922335, 0.042693, 2.473645 };

float wfc2[20]={ -8.332183,  -3.040542,  3.758681,  -4.049425,  3.050940,
-0.894016,  -1.446080,  6.029086,  5.837497,  -1.206468,
-2.927471,  -4.145836,  1.903108,  3.409983,  0.191798,
-4.116436,  -7.171240,  2.687001,  -1.662798,  3.236128 };

float bfc2[4]={ -0.894016, -2.927471, -4.116436, 4.911131 };

float wfc3[20]={ 5.578862,  5.071550,  6.798682,  -0.259254,
10.556403,  -9.337465,  -0.761154,  2.529015,
-11.307169,  -8.039979,  -2.417081,  9.576913,
-2.461208,  -6.955447,  -1.412636,  -12.198679,
-2.784967,  8.339398,  -1.883454,  -9.724778 };

float bfc3[5]={ -12.202182, -8.269168, -6.212107, 3.503773, -3.755674 };



float img[25] = {255,0,0,0,255,255,0,0,0,255,255,0,0,0,255,255,0,0,255,255,255,255,255,0,0};

float imgA[25] = {255,255,255,255,255,255,0,0,0,255,255,255,255,255,255,255,0,0,0,255,255,0,0,0,255};
float imgE[25] = {255,255,255,255,255,255,0,0,0,0,255,255,255,255,255,255,0,0,0,0,255,255,255,255,255};
float imgI[25] = {255,255,255,255,255,0,0,255,0,0,0,0,255,0,0,0,0,255,0,0,255,255,255,255,255};
float imgO[25] = {255,255,255,255,255,255,0,0,0,255,255,0,0,0,255,255,0,0,0,255,255,255,255,255,255};
float imgU[25] = {255,0,0,0,255,255,0,0,0,255,255,0,0,0,255,255,0,0,0,255,255,255,255,255,255};


int main() {
checkDatasetSSCover(wfc1, bfc1, wfc2, bfc2, wfc3, bfc3);

}
