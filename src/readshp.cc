// // #include "gdal/gdal_priv.h"

#include <iostream>
#include <iterator>
#include <ogrsf_frmts.h>

// int main()

// {
//     GDALAllRegister();

//     GDALDataset *poDS = static_cast<GDALDataset*>(
//         GDALOpenEx( "LNDCVR_California_State_Shape(1)/Shape/LandCover_Woodland.shp", GDAL_OF_VECTOR, NULL, NULL, NULL ));
//     if( poDS == NULL )
//     {
//         printf( "Open failed.\n" );
//         exit( 1 );
//     }


//     OGRLayer  *poLayer = poDS->GetLayer(0);
//     // std::cout << poLayer << std::endl;

//     // poLayer->ResetReading();
//     // OGRFeature *poFeature;
//     // while( (poFeature = poLayer->GetNextFeature()) != NULL )
//     // {
//     //     for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
//     //     {
//     //         OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

//     //         switch( poFieldDefn->GetType() )
//     //         {
//     //             // case OFTInteger:
//     //             //     printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
//     //             //     break;
//     //             // case OFTInteger64:
//     //             //     printf( CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64( iField ) );
//     //             //     break;
//     //             // case OFTReal:
//     //             //     printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
//     //             //     break;
//     //             // case OFTString:
//     //             //     printf( "%s,", poFeature->GetFieldAsString(iField) );
//     //             //     break;
//     //             // default:
//     //             //     printf( "%s,", poFeature->GetFieldAsString(iField) );
//     //             //     break;
//     //         }
//     //     }

//     //     OGRGeometry *poGeometry = poFeature->GetGeometryRef();
//     //     OGRCoordinateTransformation *coordinates;

//     //     // poGeometry->toPoint()->transform(coordinates);


//     //     std::cout << poGeometry->toPoint()->getPoints() << std::endl;
//     //     OGRFeature::DestroyFeature( poFeature );
//     // }

//     GDALClose( poDS );
// }

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

    // while( (poFeature = poLayer->GetNextFeature()) != NULL )
    // {
    	// std::cout << poFDefn->GetFieldCount() << std::endl;

        // for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
        // {
        //     OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

        //     switch( poFieldDefn->GetType() )
        //     {
        //         case OFTInteger:
        //             printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
        //             break;
        //         case OFTInteger64:
        //             printf( CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64( iField ) );
        //             break;
        //         case OFTReal:
        //             printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
        //             break;
        //         case OFTString:
        //             printf( "%s,", poFeature->GetFieldAsString(iField) );
        //             break;
        //         default:
        //             printf( "%s,", poFeature->GetFieldAsString(iField) );
        //             break;
        //     }
        // }

        // OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        // if( poGeometry != NULL
        //         && wkbFlatten(poGeometry->getGeometryType()) == wkbPoint )
        // {
        //     OGRPoint *poPoint = (OGRPoint *) poGeometry;

        //     printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
        // }
        // else
        // {
        //     printf( "no point geometry\n" );
        // }
        // OGRFeature::DestroyFeature( poFeature );
    // }

    GDALClose( poDS );
}
