#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"

int main()
{
    GDALDataset  *poDataset;
    GDALAllRegister();
    // put here your downloaded EVT files
    const char *input = "/Users/adilettuleuov/Downloads/lf19715421_US_140EVT";

    GDALDriver *pDriverAIG;

    pDriverAIG = GetGDALDriverManager() -> GetDriverByName("AIG");

    poDataset = (GDALDataset *) GDALOpen( input, GA_ReadOnly );
    if( poDataset == NULL )
    {
        std::printf("Error");
    }
}
