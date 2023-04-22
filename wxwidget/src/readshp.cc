// // #include "gdal/gdal_priv.h"

#include <iostream>
#include <iterator>
#include <ogrsf_frmts.h>

int main()

{
    GDALAllRegister();

    GDALDataset *poDS = static_cast<GDALDataset*>(
        GDALOpenEx( "LNDCVR_California_State_Shape(1)/Shape/LandCover_Woodland.shp", GDAL_OF_VECTOR, NULL, NULL, NULL ));
    if( poDS == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }

    OGRLayer  *poLayer = poDS->GetLayer( 0 );
    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();

    poLayer->ResetReading();
    OGRFeature *poFeature;
    std::cout << poLayer->GetSpatialRef()->GetName() << std::endl;


    GDALClose( poDS );
}
