#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"

int main()
{
    GDALDataset  *poDataset;
    GDALAllRegister();
    // const char *input = "/Users/adilettuleuov/Downloads/lf75238921_US_140EVT/US_140EVT\\us_140evt\\w001001.adf";
    const char *input = "/Users/adilettuleuov/Downloads/lf19715421_US_140EVT";

    GDALDriver *pDriverAIG;

    pDriverAIG = GetGDALDriverManager() -> GetDriverByName("AIG");

    poDataset = (GDALDataset *) GDALOpen( input, GA_ReadOnly );
    if( poDataset == NULL )
    {
        std::printf("Error");
    }
}
