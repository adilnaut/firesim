
#include "wx/wx.h"
#include <wx/glcanvas.h>
#include <iostream>

#ifdef __WXMAC__
#include <GLUT/glut.h>
#include "gdal_priv.h"
#include "cpl_conv.h"
#else
#include <GL/glut.h>
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h"
#endif


#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>
#include <vector>
#include <stdlib.h>

int simulation = 0;
std::vector<float> canvasdata(100 * 100, 0.0f);

// 1. "Forest"  темно зеленый
//  2. "Woodland":  чуть светлее зеленый
//  3. "Grassland": салатовый
// 4.   "Soil": коричневый
// 5.  "Water": голубой
// 6. "Fire": красный


// 1: 0, 0.5, 0
// 2: 0, 1, 0
// 3. 0.25, 1, 0.25
// 4: 1, 50, 25
// 5: 0.1, 0.9, 0.9
// 6: 1 , 0, 0

// canvasdata может принимать значения от 1 до 6
void FireSimulationStep(){
   std::vector<float> canvas_old(100 * 100, 0.0f);
   canvas_old = canvasdata;
   for(int i = 1; i < 99; i++){
      for(int j = 1 ; j < 99; j++){
        int v2 = rand() % 100;
        if (v2 > 50){
          continue;
        }

        if ( (canvas_old[i + 100*j] == 6) || (canvas_old[i + 100*j] == 5) ){
           continue;
        }
         float neighbour1 = canvas_old[(i-1) + 100*j];
         float neighbour2 = canvas_old[(i+1) + 100*j];
         float neighbour3 = canvas_old[i + 100*(j-1)];
         float neighbour4 = canvas_old[i + 100*(j+1)];

         // float gradient1 = (dem_data[i + 100*j] - dem_data[(i-1) + 100*j]);
         // float gradient2 = (dem_data[i + 100*j] - dem_data[(i+1) + 100*j]);
         // float gradient3 = (dem_data[i + 100*j] - dem_data[i + 100*(j-1)]);
         // float gradient4 = (dem_data[i + 100*j] - dem_data[i + 100*(j+1)]);

         float gradient1 = 1;
         float gradient2 = 1;
         float gradient3 = 1;
         float gradient4 = 1;

         // если градиент положительный, то огню идти легче
         // если отрицательный, то тяжелее


         int v1 = rand() % 100;
         int fire = 0;
         if (neighbour1 == 6.0)  {
           if (gradient1 > 0){
             fire = 2;
           } else{
             fire = 1;
           }
         }
         if (neighbour2 == 6.0)  {
           if (gradient2 > 0){
             fire = 2;
           } else{
             fire = 1;
           }
         }
         if (neighbour3 == 6.0)  {
           if (gradient3 > 0){
             fire = 2;
           } else{
             fire = 1;
           }
         }
         if (neighbour4 == 6.0)  {
           if (gradient4 > 0){
             fire = 2;
           } else{
             fire = 1;
           }
         }

         if (fire = 1){
           if (canvas_old[i + 100*j] == 1){
              if (v1 < 65){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 2){
              if (v1 < 55){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 1){
              if (v1 < 45){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 1){
              if (v1 < 25){
                canvas_old[i + 100*j] = 6;
              }
           }

         }

         if (fire = 2){
           if (canvas_old[i + 100*j] == 1){
              if (v1 < 75){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 2){
              if (v1 < 65){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 1){
              if (v1 < 55){
                canvas_old[i + 100*j] = 6;
              }
           }

           if (canvas_old[i + 100*j] == 1){
              if (v1 < 35){
                canvas_old[i + 100*j] = 6;
              }
           }

         }



      }
   }
}


class GLCanvas: public wxGLCanvas{
  public:
    GLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
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

  void GLCanvas::Render(wxString example_text) {
  SetCurrent(*context);
    if (!init) {
      InitGL();
      init = true;
    }

    // readTer(terdir);
    drawCanvas();

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

    // DemSimulation = 0;
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

class ToolFrame: public wxFrame
{
 public:
  ToolFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,const wxSize& size);

       //drawDot("ola");
 private:
  void OnStart(wxCommandEvent& event) {
      simulation = 1;
       std::cout << "Button Start Pressed" << std::endl;
  }
  void OnPause(wxCommandEvent& event) {
      simulation = 0;
      std::cout << "Button Pause Pressed" << std::endl;
  }
  void OnStop(wxCommandEvent& event) {
    simulation = 0;
    std::cout << "Button Stop Pressed" << std::endl;
  }
  void OnExit(wxCommandEvent& event) { Close(true); }
  void OnMouseEvent(wxMouseEvent& event) {
    const wxPoint pt = wxGetMousePosition();
    int mouseX = pt.x - this->GetScreenPosition().x;
    int mouseY = pt.y - this->GetScreenPosition().y;
    mouseX = (mouseX* 100)/800;
    mouseY = (mouseY * 100)/800;
    printf("Mouse Event %i %i\n", mouseX, mouseY);
    // drawPoint();
    // drawCanvas();
    // update = 0;
 }

  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ToolFrame, wxFrame)

  EVT_MENU(wxID_EXIT, ToolFrame::OnExit)
  EVT_BUTTON(3, ToolFrame::OnStart)
  EVT_BUTTON(4, ToolFrame::OnPause)
  EVT_BUTTON(5, ToolFrame::OnStop)
  EVT_LEFT_UP(ToolFrame::OnMouseEvent)

END_EVENT_TABLE()

  ToolFrame::ToolFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size):
  wxFrame(parent, wxID_ANY, title, pos, size)
{


  wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);

  button_sizer->Add(new wxButton(this, 3,  "Start"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 4,  "Pause"), 0, wxALL, 10);
  button_sizer->Add(new wxButton(this, 5,  "Stop"), 0, wxALL, 10);

  SetSizer(button_sizer);


}


class MyApp: public wxApp
{
    // wxFrame *frame;
    // Canvas* canvas;
public:
    bool OnInit()
    {

        char **tmp1; int tmp2 = 0; glutInit(&tmp2, tmp1);
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        ToolFrame *toolFrame = new ToolFrame(NULL,  "Tools", wxDefaultPosition, wxSize(1050, 700));
        ScrolledImageComponent* my_image = new ScrolledImageComponent(frame, wxID_ANY );
        sizer->Add(my_image, 1, wxALL | wxEXPAND, 60);

        toolFrame->SetSizer(sizer);


        toolFrame->Show();

        // MyFrame *frame = new MyFrame(NULL,  "Main", wxDefaultPosition, wxSize(800, 800));
        // // wxFrame *frame = new wxFrame((wxFrame *)NULL, -1,  wxT("Elevation map"), wxPoint(50,50), wxSize(800,800));
        //
        // ScrolledImageComponent* my_image = new ScrolledImageComponent(frame, wxID_ANY );
        // sizer->Add(my_image, 1, wxALL | wxEXPAND, 60);
        // frame->SetSizer(sizer);
        // canvas= new Canvas(my_image, wxID_ANY, wxDefaultPosition,  wxSize(800,800));
        // frame->Show();



        return true;
    }
};

IMPLEMENT_APP(MyApp)
