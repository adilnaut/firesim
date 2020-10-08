#include "wx/wx.h"
#include <wx/glcanvas.h>
#include <iostream>
#include <wx/wx.h>
#include <GLUT/glut.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <vector>
#include "gdal_priv.h"
#include "cpl_conv.h"

#include <json/value.h>
#include <json/reader.h>
// #include "json/json.h"

#include <fstream>
#include <string>


int DemSimulation = 0;
int simulation = 0;
int demread = 0;
int terread = 0;
int mouseX = -1;
int mouseY = -1;
std::vector<float> demdata(10812 * 10812, 0.0f);
std::vector<float> terdata(22336 * 37774, 0.0f);
std::vector<float> canvasdata(100 * 100, 0.0f);

//Эта функция читает файл и сразу рисует
void readDem(wxString dir){
   GDALAllRegister();

    // Open the file
    GDALDataset  *dataset = (GDALDataset *) GDALOpen(dir, GA_ReadOnly);
    if( dataset == NULL ) {
      std::cout << "Failed to open" << std::endl;
      exit(1);
    }

    // Get image metadata
    unsigned width = dataset->GetRasterXSize();
    unsigned height = dataset->GetRasterYSize();

    std::cout << "Image is " << width << " x " << height << " px" << std::endl;

    double originX, originY, pixelSizeX, pixelSizeY;

    double geoTransform[6];
    if (dataset->GetGeoTransform(geoTransform) == CE_None ) {
        originX = geoTransform[0];
        originY = geoTransform[3];
        pixelSizeX = geoTransform[1];
        pixelSizeY = geoTransform[5];
    } else {
      std::cout << "Failed read geotransform" << std::endl;
      exit(1);
    }

    std::cout << "Origin: " << originX << ", " << originY << " degrees" << std::endl;
    std::cout << "Pixel size: " << pixelSizeX << ", " << pixelSizeY << " degrees" << std::endl;

    // pixelSizeY is negative because the origin of the image is the north-east corner and positive
    // Y pixel coordinates go towards the south

    // Get the raster band (DEM has one raster band representing elevation)
    GDALRasterBand  *elevationBand = dataset->GetRasterBand(1);

    // Create an array of width*height 32-bit floats (~400MB memory)
    // std::vector<float> data(width * height, 0.0f);

    // Read the entire file into the array (you can change the options to read only a portion
    // of the file, or even scale it down if you want)

    std::cout << "Loading array..." << std::flush;
    if (elevationBand -> RasterIO(GF_Read, 0, 0, width, height, &demdata[0], width, height, GDT_Float32, 0, 0)) {
        // printf("%s\n", "h");
    }
    std::cout << "done" << std::endl;

    // Close the file
    GDALClose(dataset);
}

void readTer(wxString dir){
  GDALAllRegister();

  // Open the file
  GDALDataset  *dataset = (GDALDataset *) GDALOpen(dir, GA_ReadOnly);
  if( dataset == NULL ) {
    std::cout << "Failed to open" << std::endl;
    exit(1);
  }

  // Get image metadata
  unsigned width = dataset->GetRasterXSize();
  unsigned height = dataset->GetRasterYSize();
  // printf("%i\n", width);
  // printf("%i\n", height);
  std::cout << "Image is " << width << " x " << height << " px" << std::endl;

  double originX, originY, pixelSizeX, pixelSizeY;

  double geoTransform[6];

  if (dataset->GetGeoTransform(geoTransform) == CE_None ) {
      // printf("%lf\n", geoTransform[0]);
      // printf("%lf\n", geoTransform[1]);
      // printf("%lf\n", geoTransform[2]);
      // printf("%lf\n", geoTransform[3]);
      // printf("%lf\n", geoTransform[4]);
      // printf("%lf\n", geoTransform[5]);
      originX = geoTransform[0];
      originY = geoTransform[3];
      pixelSizeX = geoTransform[1];
      pixelSizeY = geoTransform[5];
      // originX = geoTransform[1];
      // originY = geoTransform[5];
      // pixelSizeX = geoTransform[0];
      // pixelSizeY = geoTransform[3];
  } else {
    std::cout << "Failed read geotransform" << std::endl;
    exit(1);
  }

  printf( "Driver: %s/%s\n",
      dataset->GetDriver()->GetDescription(),
      dataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
  printf( "Size is %dx%dx%d\n",
      dataset->GetRasterXSize(), dataset->GetRasterYSize(),
      dataset->GetRasterCount() );

  // if( dataset->GetProjectionRef()  != NULL )
  //   printf( "Projection is `%s'\n", dataset->GetProjectionRef() );

  // printf("%f\n", originX);
  // printf("%f\n", originY);

  std::cout << "Origin: " << originX << ", " << originY << " degrees" << std::endl;
  std::cout << "Pixel size: " << pixelSizeX << ", " << pixelSizeY << " degrees" << std::endl;

  // pixelSizeY is negative because the origin of the image is the north-east corner and positive
  // Y pixel coordinates go towards the south

  // Get the raster band (DEM has one raster band representing elevation)
  GDALRasterBand  *elevationBand = dataset->GetRasterBand(1);
  int             nBlockXSize, nBlockYSize;
  int             bGotMin, bGotMax;
  double          adfMinMax[2];


  // Create an array of width*height 32-bit floats (~400MB memory)


  // Read the entire file into the array (you can change the options to read only a portion
  // of the file, or even scale it down if you want)

  // elevationBand = dataset->GetRasterBand( 1 );
  elevationBand->GetBlockSize( &nBlockXSize, &nBlockYSize );

  printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
      nBlockXSize, nBlockYSize,
      GDALGetDataTypeName(elevationBand->GetRasterDataType()),
      GDALGetColorInterpretationName(
          elevationBand->GetColorInterpretation()) );
  adfMinMax[0] = elevationBand->GetMinimum( &bGotMin );
  adfMinMax[1] = elevationBand->GetMaximum( &bGotMax );
  if( ! (bGotMin && bGotMax) )
      GDALComputeRasterMinMax((GDALRasterBandH)elevationBand, TRUE, adfMinMax);
  printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
  if( elevationBand->GetOverviewCount() > 0 )
      printf( "Band has %d overviews.\n", elevationBand->GetOverviewCount() );
  if( elevationBand->GetColorTable() != NULL )
      printf( "Band has a color table with %d entries.\n",
              elevationBand->GetColorTable()->GetColorEntryCount() );

  std::cout << "Loading array...\n" << std::flush;
  float *pafScanline;
  int   nXSize = elevationBand->GetXSize();
  int   nYSize = elevationBand->GetYSize();
  pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

  elevationBand->RasterIO( GF_Read, 0, 0, nXSize, 1, pafScanline, nXSize, 1, GDT_Float32, 0, 0 );

  for (int i =0; i < nXSize; i++){
    printf("%f\n", pafScanline[i]);
  }


  if (elevationBand -> RasterIO(GF_Read, 0, 0, width, height, &terdata[0], width, height, GDT_Float32, 0, 0)) {
      // printf("%s\n", "h");
  }

  std::cout << "done" << std::endl;

  // Close the file
  GDALClose(dataset);
}
    // Be careful with axis order: coordinates are traditionally written in lat, lon order, but
    // those are Y, X, respectively.

    // double pointLat = 37.5000900;
    // double pointLon = -121.5000000;

    // double pixelX = (pointLon - originX) / pixelSizeX;
    // double pixelY = (pointLat - originY) / pixelSizeY;

    // std::cout << pointLat << "," << pointLon << " maps to pixel " << pixelX << "," << pixelY << std::endl;

    // if (pixelX < 0 || pixelX > width || pixelY < 0 || pixelY > height) {
    //   std::cout << "Point out of bounds!" << std::endl;
    //   exit(1);
    // }
    //Начинает рисовать тут
void drawDem(){
    glClear(GL_COLOR_BUFFER_BIT);
    int left = 0, top = 800;
    int right = 10, bottom = 790;
    double color = 0;
    for( int w = 0; w < 10800; w = w + 108 ) {
        for (int h = 0; h < 10800; h = h + 108){
            //THESE FORMULAS FOR GETTING LATandLONG by pixels
            // double pointLatt = (w*pixelSizeX) + originX;
            // double pointLong = (h*pixelSizeY) + originY;
            //Псс, это то что тебе нужно
            //data[h + w*width] это достает высоту
            //смотри, у тебя 100х100, чтобы достать первый пиксель (1х1) это w=108 and h=108, and for (2x2) w=216 and h=216, so 108 is a step
            // why w*width? потому что этот лист одномерный и просто скипаешь всю широту (width)
            double elev = demdata[h + w*10812];

            if (elev < 70){
              glColor3f(0.0, 0.1, 0.0);
            }
            if (elev > 100 && elev < 200) {
              glColor3f(0.0, 0.25, 0.0);
            }
            if (elev > 200 && elev < 300) {
              glColor3f(0.0, 0.5, 0.0);
            }
            if (elev > 300 && elev < 400) {
              glColor3f(0.0, 0.75, 0.0);
            }
            if (elev > 400 && elev < 500) {
              glColor3f(0.0, 1.0, 0.0);
            }
            if (elev > 500 && elev < 600) {
              glColor3f(0.25, 1.0, 0.25);
            }
            // int elev_int = (int) elev;
            //
            // std::string s = std::to_string(elev_int);
            // // std::cout << "log 1" << std::endl;
            //
            // Json::Reader reader;
            // Json::Value labels;
            // std::ifstream file_input("data.json");
            // reader.parse(file_input, labels);
            // // std::cout << elev << std::endl;
            //
            // // setcolor(color);
            // // rectangle(left, top, right, bottom);
            // try
            // {
            //     std::cout << labels[s].asString() << std::endl;
            // }
            // catch (const std::exception& err)
            // {
            //   std::cout << "labels[s].asString()" << std::endl;
            //   glColor3f(1.0, 1.0 , 1.0);
            //   continue;
            //     // std::cout << "Error!!!" << std::endl;
            // }
            // if (labels[s].asString() == "Forest"){
            //   glColor3f(0.0, 0.5, 0.0);
            // }
            //
            // if (labels[s].asString() == "Woodland") {
            //   glColor3f(0.0, 1.0, 0.0);
            // }
            // if (labels[s].asString() == "Grassland") {
            //   glColor3f(0.25, 1.0, 0.25);
            // }
            // if (labels[s].asString() == "Soil") {
            //   glColor3f(1.0, 0.5, 0.25);
            // }
            // if (labels[s].asString() == "Water") {
            //   glColor3f(0.1, 0.9, 0.9);
            // }
            // else {
            //   glColor3f(1.0, 1.0 , 1.0);
            // }
            // if el(ev > 70 && elev < 75) {
            //   glColor3f(0.5, 1.0, 0.5);
            // }
            // if (elev > 75) {
            //   glColor3f(0.75, 1.0, 0.75);
            // }

            glBegin(GL_QUADS);
            // glTexCoord2f(0, 10);
            glVertex2f(left, top);
            // glTexCoord2f(0, 0);
            glVertex2f(left, bottom);
            // glTexCoord2f(10, 0);
            glVertex2f(right, bottom);
            // glTexCoord2f(10, 10);
            glVertex2f(right, top);
            glEnd();


            left = left + 10;
            right = right + 10;
        }
        left = 0;
        right = 10;
        top = top - 10;
        bottom = bottom -10;
        // std::cout << left << " " << right << " " << top << " " << bottom << std::endl;
    }






    // glBegin(GL_LINE_STRIP);

    //   for (int i=0; i<6; i++) {
    //     if (i%2) y = 10.0;
    //     else y = 30.0;
    //     glVertex2f(20*i+10.0, y);
    //     glVertex2f(20*i+30.0, y);
    //   }
    // glEnd();
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos2f(10, 100);
}

void drawTer(){
    glClear(GL_COLOR_BUFFER_BIT);
    int left = 0, top = 800;
    int right = 10, bottom = 790;
    double color = 0;
    for( int w = 5000; w < 5100; w = w + 1 ) {
        for (int h = 5000; h < 5100; h = h + 1){
          //WIDTH OF TERDATA INSTEAD OD WIDTH
            double elev = terdata[h + w*37774];
            if (elev < 3000){
              glColor3f(0.0, 0.1, 0.0);
            }
            if (elev > 3150 && elev < 3300) {
              glColor3f(0.0, 0.25, 0.0);
            }
            if (elev > 3300 && elev < 3600) {
              glColor3f(0.0, 0.5, 0.0);
            }
            if (elev > 3600 && elev < 3700) {
              glColor3f(0.0, 0.75, 0.0);
            }
            if (elev > 3700 && elev < 3800) {
              glColor3f(0.0, 1.0, 0.0);
            }
            if (elev > 3800 && elev < 3989) {
              glColor3f(0.25, 1.0, 0.25);
            }

            glBegin(GL_QUADS);
            // glTexCoord2f(0, 10);
            glVertex2f(left, top);
            // glTexCoord2f(0, 0);
            glVertex2f(left, bottom);
            // glTexCoord2f(10, 0);
            glVertex2f(right, bottom);
            // glTexCoord2f(10, 10);
            glVertex2f(right, top);
            glEnd();


            left = left + 10;
            right = right + 10;
        }
        left = 0;
        right = 10;
        top = top - 10;
        bottom = bottom -10;
    }
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos2f(10, 100);
}

void drawPoint(){
  canvasdata[mouseY + mouseX*100] = 1.1;
}

void drawCanvas(){
  glClear(GL_COLOR_BUFFER_BIT);
  int left = 0, top = 800;
  int right = 10, bottom = 790;
  double color = 0;
  for( int w = 0; w < 10800; w = w + 108 ) {
      for (int h = 0; h < 10800; h = h + 108){
        double point = canvasdata[h + w*100];
        if (point == 1.1){
          glColor3f(1.0, 0.0, 0.0);
        }
        else{
          glColor3f(0.0, 0.0, 0.0);
        }

        glBegin(GL_QUADS);
        // glTexCoord2f(0, 10);
        glVertex2f(left, top);
        // glTexCoord2f(0, 0);
        glVertex2f(left, bottom);
        // glTexCoord2f(10, 0);
        glVertex2f(right, bottom);
        // glTexCoord2f(10, 10);
        glVertex2f(right, top);
        glEnd();


        left = left + 10;
        right = right + 10;
    }
    left = 0;
    right = 10;
    top = top - 10;
    bottom = bottom -10;
  }
  glColor3f(0.0, 0.0, 1.0);
  glRasterPos2f(10, 100);

}

class DemGLCanvas: public wxGLCanvas{
  public:
    DemGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    void Render(wxString example_text = ""); // function to draw canvas contents
  private:
    wxGLContext *context;              // OpenGL rendering context
    bool init;                         // has the OpenGL context been initialised?
    int pan_x;                         // the current x pan
    int pan_y;                         // the current y pan
    double zoom;                       // the current zoom
    void InitGL();                     // function to initialise OpenGL context
    void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
    void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed
    DECLARE_EVENT_TABLE()
  };

  void DemGLCanvas::Render(wxString example_text) {
  SetCurrent(*context);
    if (!init) {
      InitGL();
      init = true;
    }
    //ПОМЕНЯЙ ДИР
    // readDem("/home/arman/Downloads/USGS_NED_13_n38w122_ArcGrid/grdn38w122_13");
    drawDem();
    for (int i = 0; i < example_text.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

    // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
    glFlush();
    SwapBuffers();
  }

  void DemGLCanvas::InitGL()
    // Function to initialise the GL context
  {
    int w, h;
    GetClientSize(&w, &h);
    SetCurrent(*context);
    glDrawBuffer(GL_BACK);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(pan_x, pan_y, 0.0);
    glScaled(zoom, zoom, zoom);
  }

  void DemGLCanvas::OnPaint(wxPaintEvent& event)
    // Event handler for when the canvas is exposed
  {
    int w, h;
    wxString text;
    wxPaintDC dc(this); // required for correct refreshing under MS windows
    GetClientSize(&w, &h);
    //text.Printf("by OnPaint event handler, canvas size is %d by %d"S, w, h);
    if (DemSimulation == 1){
      Render(text);
    }

    DemSimulation = 0;
  }

  void DemGLCanvas::OnSize(wxSizeEvent& event)
    // Event handler for when the canvas is resized
  {
    init = false; // this will force the viewport and projection matrices to be reconfigured on the next paint
  }

  BEGIN_EVENT_TABLE(DemGLCanvas, wxGLCanvas)
    EVT_SIZE(DemGLCanvas::OnSize)
    EVT_PAINT(DemGLCanvas::OnPaint)
  END_EVENT_TABLE()

  int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

  DemGLCanvas::DemGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size):
    wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size)
    // Constructor - initialises private variables
  {
    context = new wxGLContext(this);
    init = false;
    pan_x = 0;
    pan_y = 0;
    zoom = 1.0;
  }

class TerGLCanvas: public wxGLCanvas{
  public:
    TerGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
    void Render(wxString example_text = ""); // function to draw canvas contents
  private:
    wxGLContext *context;              // OpenGL rendering context
    bool init;                         // has the OpenGL context been initialised?
    int pan_x;                         // the current x pan
    int pan_y;                         // the current y pan
    double zoom;                       // the current zoom
    void InitGL();                     // function to initialise OpenGL context
    void OnSize(wxSizeEvent& event);   // event handler for when canvas is resized
    void OnPaint(wxPaintEvent& event); // event handler for when canvas is exposed
    DECLARE_EVENT_TABLE()
  };

  void TerGLCanvas::Render(wxString example_text) {
  SetCurrent(*context);
    if (!init) {
      // InitGL();
      // init = true;
    }
    //ПОМЕНЯЙ ДИР
    // readDem("/home/arman/Downloads/USGS_NED_13_n38w122_ArcGrid/grdn38w122_13");
    drawCanvas();
    for (int i = 0; i < example_text.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

    // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
    glFlush();
    SwapBuffers();
  }

  void TerGLCanvas::InitGL()
    // Function to initialise the GL context
  {
    int w, h;
    GetClientSize(&w, &h);
    SetCurrent(*context);
    glDrawBuffer(GL_BACK);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(pan_x, pan_y, 0.0);
    glScaled(zoom, zoom, zoom);
  }

  void TerGLCanvas::OnPaint(wxPaintEvent& event)
    // Event handler for when the canvas is exposed
  {
    int w, h;
    wxString text;
    wxPaintDC dc(this); // required for correct refreshing under MS windows
    GetClientSize(&w, &h);
    //text.Printf("by OnPaint event handler, canvas size is %d by %d"S, w, h);
    if (simulation == 1){
      Render(text);
    }

  }



  BEGIN_EVENT_TABLE(TerGLCanvas, wxGLCanvas)

    EVT_PAINT(TerGLCanvas::OnPaint)
  END_EVENT_TABLE()



  TerGLCanvas::TerGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size):
    wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size)
    // Constructor - initialises private variables
  {
    context = new wxGLContext(this);
    init = false;
    pan_x = 0;
    pan_y = 0;
    zoom = 1.0;
  }

class ScrolledImageComponent : public wxScrolledWindow
  {
       int w,h;
  public:
      ScrolledImageComponent(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id)
      {
           /* init scrolled area size, scrolling speed, etc. */
          SetScrollbars(1,1, 800, 800, 0, 0);
      }
};

 class MyFrame: public wxFrame
{
 public:
  MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
          const wxSize& size);
  private:
    void OnMouseEvent(wxMouseEvent& event) {
      const wxPoint pt = wxGetMousePosition();
      mouseX = pt.x - this->GetScreenPosition().x;
      mouseY = pt.y - this->GetScreenPosition().y;
      drawPoint();
      drawCanvas();
      // update = 0;
  }
  DECLARE_EVENT_TABLE()

};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_LEFT_UP(MyFrame::OnMouseEvent)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
         const wxSize& size):
  wxFrame(parent, wxID_ANY, title, pos, size)
{


}


class ToolFrame: public wxFrame
{
 public:
  ToolFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
          const wxSize& size);

       //drawDot("ola");
 private:
  void OnStart(wxCommandEvent& event) {
      simulation = 1;
      DemSimulation = 1;
       std::cout << "Button 1 Pressed" << std::endl;
  }
  void OnPause(wxCommandEvent& event) {
      simulation = 0;
       std::cout << "Button 1 Pressed" << std::endl;
  }
  void OnStop(wxCommandEvent& event) {
    simulation = 0;
    // canvasdata = originalCanvas;
       std::cout << "Button 1 Pressed" << std::endl;
  }
  void OnExit(wxCommandEvent& event) { Close(true); }
  void OnButton2(wxCommandEvent& event) {
       std::cout << "Button 2 Pressed" << std::endl;
  }
  void DemFilePicker(wxCommandEvent&  WXUNUSED(event)) {
      // wxFileDialog openFileDialog(this, _("Open dem directory"), "", "", "", wxFD_OPEN); //|wxFD_FILE_MUST_EXIST
      // if (openFileDialog.ShowModal() == wxID_CANCEL)
      //   return;     // the user changed idea...
      // readDem(openFileDialog.GetDirectory());
      readDem("/Users/adilettuleuov/Downloads/USGS_NED_13_n38w122_ArcGrid/grdn38w122_13");
      // readDem("/Users/adilettuleuov/Downloads/lf19715421_US_140EVT");
      // std::cout << "File chosen="  << openFileDialog.GetDirectory() << std::endl;
      drawDem();
  }
  void TerFilePicker(wxCommandEvent&  WXUNUSED(event)) {
      // wxFileDialog openFileDialog(this, _("Open terrain directory"), "", "", "", wxFD_OPEN); //|wxFD_FILE_MUST_EXIST
      // if (openFileDialog.ShowModal() == wxID_CANCEL)
      //   return;     // the user changed idea...
      // readTer(openFileDialog.GetDirectory());
      readTer("/Users/adilettuleuov/Downloads/lf19715421_US_140EVT");
      // std::cout << "File chosen="  << openFileDialog.GetDirectory() << std::endl;
      drawTer();
  }

  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ToolFrame, wxFrame)
  EVT_MENU(wxID_EXIT, ToolFrame::OnExit)
  EVT_BUTTON(1, ToolFrame::DemFilePicker)
  EVT_BUTTON(2, ToolFrame::TerFilePicker)
  EVT_BUTTON(3, ToolFrame::OnStart)
  EVT_BUTTON(4, ToolFrame::OnPause)
  EVT_BUTTON(5, ToolFrame::OnStop)

END_EVENT_TABLE()

ToolFrame::ToolFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
         const wxSize& size):
  wxFrame(parent, wxID_ANY, title, pos, size)
{
  wxMenu *fileMenu = new wxMenu;
  // The '&' in the next line underlines the succeeding character
  fileMenu->Append(wxID_EXIT,  "&Quit");
  wxMenuBar *menuBar = new wxMenuBar;

  menuBar->Append(fileMenu,  "&File");
  SetMenuBar(menuBar);

  wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);

  button_sizer->Add(new wxButton(this, 1,  "Open Elevation Data"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 2,  "Open Terrain Data"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 3,  "Start"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 4,  "Pause"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 5,  "Stop"), 0, wxALL, 10);

  SetSizer(button_sizer);


}


class MyApp: public wxApp
{
    // wxFrame *frame;
  DemGLCanvas* canvas;
  TerGLCanvas* canvas2;
public:
    bool OnInit()
    {
        // wxButton* fire;
        char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        ToolFrame *toolFrame = new ToolFrame(NULL,  "Testing", wxDefaultPosition, wxSize(1050, 100));
    //    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Scrolling an Image"), wxPoint(50,50), wxSize(650,650));


        toolFrame->Show();

        MyFrame *frame = new MyFrame(NULL,  "Testing", wxDefaultPosition, wxSize(800, 800));
        // wxFrame *frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Elevation map"), wxPoint(50,50), wxSize(800,800));

        ScrolledImageComponent* my_image = new ScrolledImageComponent(frame, wxID_ANY );
        sizer->Add(my_image, 1, wxALL | wxEXPAND, 60);
        frame->SetSizer(sizer);
        canvas= new DemGLCanvas(my_image, wxID_ANY, wxDefaultPosition,  wxSize(800,800));
        frame->Show();


        MyFrame *frame2 = new MyFrame(NULL,  "Testing", wxDefaultPosition, wxSize(800, 800));
        // wxFrame *frame2 = new wxFrame((wxFrame *)NULL, -1,  wxT("Scrolling an Image"), wxPoint(50,50), wxSize(800,800));

        ScrolledImageComponent* my_image2 = new ScrolledImageComponent(frame2, wxID_ANY );
        sizer->Add(my_image2, 1, wxALL | wxEXPAND, 60);
        frame2->SetSizer(sizer);
        canvas2= new TerGLCanvas(my_image2, wxID_ANY, wxDefaultPosition,  wxSize(800,800));
        frame2->Show();

        return true;
    }
};

IMPLEMENT_APP(MyApp)
