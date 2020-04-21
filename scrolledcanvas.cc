#include "wx/wx.h"
#include <wx/glcanvas.h>
#include <iostream>
#include <wx/wx.h>
#include <GL/glut.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <vector>
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h"

int update = 0;

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
    std::vector<float> data(width * height, 0.0f);
   
    // Read the entire file into the array (you can change the options to read only a portion
    // of the file, or even scale it down if you want)
     
    std::cout << "Loading array..." << std::flush;
    if (elevationBand -> RasterIO(GF_Read, 0, 0, width, height, &data[0], width, height, GDT_Float32, 0, 0)) {
        // printf("%s\n", "h");
    }
    std::cout << "done" << std::endl;
    
    // Close the file
    GDALClose(dataset); 
    
    // Be careful with axis order: coordinates are traditionally written in lat, lon order, but
    // those are Y, X, respectively. 

    double pointLat = 37.5000900;
    double pointLon = -121.5000000;
    
    double pixelX = (pointLon - originX) / pixelSizeX;
    double pixelY = (pointLat - originY) / pixelSizeY;
    
    std::cout << pointLat << "," << pointLon << " maps to pixel " << pixelX << "," << pixelY << std::endl;
    
    if (pixelX < 0 || pixelX > width || pixelY < 0 || pixelY > height) {
      std::cout << "Point out of bounds!" << std::endl;
      exit(1);
    }
    //Начинает рисовать тут
    glClear(GL_COLOR_BUFFER_BIT);
    int curr_width = 800;
    int curr_height = 800;
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
            double elev = data[h + w*width];
            
            // std::cout << elev << std::endl;

            // setcolor(color);
            // rectangle(left, top, right, bottom); 

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

class MyGLCanvas: public wxGLCanvas{
  public:
    MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
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

  void MyGLCanvas::Render(wxString example_text) {
  SetCurrent(*context);
    if (!init) {
      InitGL();
      init = true;
    }
    std::cout << exa << std::endl;
    //ПОМЕНЯЙ ДИР
    readDem("/home/arman/Downloads/USGS_NED_13_n38w122_ArcGrid/grdn38w122_13");
    
    for (int i = 0; i < example_text.Len(); i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

    // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
    glFlush();
    SwapBuffers();
  }

  void MyGLCanvas::InitGL()
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

  void MyGLCanvas::OnPaint(wxPaintEvent& event)
    // Event handler for when the canvas is exposed
  {
    int w, h;
    wxString text;
    wxPaintDC dc(this); // required for correct refreshing under MS windows
    GetClientSize(&w, &h);
    //text.Printf("by OnPaint event handler, canvas size is %d by %d"S, w, h);
    if (update == 0){
      Render(text);
    }

    update = 1;
    
  }

  void MyGLCanvas::OnSize(wxSizeEvent& event)
    // Event handler for when the canvas is resized
  {
    init = false; // this will force the viewport and projection matrices to be reconfigured on the next paint
  }

  BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
    EVT_SIZE(MyGLCanvas::OnSize)
    EVT_PAINT(MyGLCanvas::OnPaint)
  END_EVENT_TABLE()

  int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

  MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size):
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
  void OnExit(wxCommandEvent& event) { Close(true); }   
  void OnButton1(wxCommandEvent& event) {
       std::cout << "Button 1 Pressed" << std::endl;
       readDem("ola");
  }
  void OnButton2(wxCommandEvent& event) {
       std::cout << "Button 2 Pressed" << std::endl;
  }
  void FilePicker(wxCommandEvent&  WXUNUSED(event)) {
       wxFileDialog openFileDialog(this, _("Open txt file"), "", "", //TXT files (*.txt)|*.txt
          "", wxFD_OPEN); //|wxFD_FILE_MUST_EXIST
       if (openFileDialog.ShowModal() == wxID_CANCEL)
          return;     // the user changed idea...
       readDem(openFileDialog.GetDirectory());
       std::cout << "File chosen="  << openFileDialog.GetDirectory() << std::endl;
      
  }

  DECLARE_EVENT_TABLE()
};
    
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
  EVT_MENU(wxID_EXIT, MyFrame::OnExit)
  EVT_BUTTON(1, MyFrame::OnButton1)
  EVT_BUTTON(2, MyFrame::OnButton2)
  EVT_TOOL(4,MyFrame::FilePicker)
  EVT_TOOL(6,MyFrame::OnExit)
END_EVENT_TABLE()
  
MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, 
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
  button_sizer->Add(new wxButton(this, 1,  "Fire"), 0, 
                wxALL, 10);
  // button_sizer->Add(new wxButton(this, 2,  "button 2"), 0, 
  //               wxALL, 10);
  // button_sizer->Add(new wxStaticText(this, wxID_ANY, "Some Text"), 0, 
  //               wxTOP|wxLEFT|wxRIGHT, 10);
  SetSizer(button_sizer);
  wxToolBar *toolBar = CreateToolBar();
  wxBitmap open = wxArtProvider::GetBitmap(wxART_FILE_OPEN,wxART_TOOLBAR);
  wxBitmap exit = wxArtProvider::GetBitmap(wxART_QUIT,wxART_TOOLBAR);
  wxBitmap save = wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR);
  wxBitmap b_new = wxArtProvider::GetBitmap(wxART_NEW,wxART_TOOLBAR);
 
  toolBar->AddTool(3, "New file", b_new);
  toolBar->AddTool(4, "Open file", open);
  toolBar->AddTool(5, "Save file", save);
  toolBar->AddTool(6, "Exit", exit);
  toolBar->Realize();
}
 
class MyApp: public wxApp
{
    // wxFrame *frame;
  MyGLCanvas* canvas;
public:
 
    bool OnInit()
    {
        char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        MyFrame *frame = new MyFrame(NULL,  "Testing", wxDefaultPosition, wxSize(1050, 1050));
    //    frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Scrolling an Image"), wxPoint(50,50), wxSize(650,650));
        
        ScrolledImageComponent* my_image = new ScrolledImageComponent(frame, wxID_ANY );
        sizer->Add(my_image, 1, wxALL | wxEXPAND, 60);
        frame->SetSizer(sizer);
        canvas= new MyGLCanvas(my_image, wxID_ANY, wxDefaultPosition,  wxSize(800,800));
        frame->Show();
        return true;
    } 
};
 
IMPLEMENT_APP(MyApp)
