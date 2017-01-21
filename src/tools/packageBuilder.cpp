#include "packageBuilder.h"
#include <string>

const char* PROGRAM_NAME = "Test PackageBuilder v0.0";

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
	// Show splash
	wxBitmap bitmap;
	if (bitmap.LoadFile("../../assets/textures/Background.tga", wxBITMAP_TYPE_TGA))
	{
		new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
						   500, NULL, -1, wxDefaultPosition, wxDefaultSize,
						   wxBORDER_SIMPLE | wxSTAY_ON_TOP);
	}
	wxYield();

	// Create frame
    MyFrame *frame = new MyFrame();
    frame->Show(true);

    return true;
}

MyFrame::MyFrame() :
	wxFrame(NULL, wxID_ANY, PROGRAM_NAME)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(FILE_NEW, "&New Package...\tCtrl-N",
                     "Create a new package file");
	menuFile->Append(FILE_OPEN, "&Open Package...\tCtrl-O",
					 "Open an existing package file");
	menuFile->Append(FILE_SAVE, "&Save Package...\tCtrl-S",
					 "Save changes to the current package file");
	menuFile->Append(FILE_SAVEAS, "&Save Package As...\tCtrl-W",
					 "Save the current package in a new file");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

	m_pFilePicker = new wxFilePickerCtrl(this, F_F);

	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(new wxButton(this, -1, "A Really Really Big Button"), 0, 0, 0);
	sizer->Add(new wxButton(this, -1, "Tiny Button"), 0, 0, 0);
	sizer->Add(m_pFilePicker, 0, 0, 0);
	SetSizer(sizer);

    CreateStatusBar();
    SetStatusText(wxString(_("Welcome to ")).append(PROGRAM_NAME));
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	if(m_Package.IsModified())
	{
		if(wxMessageBox(_("Exiting will lose all unsaved changes. Continue?"),
						_("Losing unsaved changes"),
						wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION) == wxNO)
		{
			return;
		}
	}

    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("This program is used to create and modify package files for TetradGame.\n\nCreated by Alex Khouderchah"),
                  _("About PackageBuilder"), wxOK | wxICON_INFORMATION );
}

void MyFrame::OnNewFile(wxCommandEvent& WXUNUSED(event))
{
}

void MyFrame::OnOpenFile(wxCommandEvent& WXUNUSED(event))
{
	static const wxChar *FILETYPES = _T(
	"Package files|*.cpk|"
	"All files|*.*"
	);

	wxFileDialog* openFileDialog =
		new wxFileDialog(this, _("Open file"), "", "", FILETYPES,
						 wxFD_OPEN, wxDefaultPosition);

	if(openFileDialog->ShowModal() == wxID_OK)
	{
		wxString path, msg;
		path.append(openFileDialog->GetDirectory());
		path.append(wxFileName::GetPathSeparator());
		path.append(openFileDialog->GetFilename());

		msg.append(_("Current package: "));
		msg.append(openFileDialog->GetFilename());

		if(!m_Package.Load(std::string(path.mb_str())))
		{
			wxMessageBox(wxString(_("Failed to load the package file:\n")).append(openFileDialog->GetFilename()),
						 _("Load failure"), wxOK | wxICON_EXCLAMATION);
			SetStatusText(wxString(_("Failed to load: ")).append(path), 0);
		}
		else
		{
			SetStatusText(wxString(_("Successfully loaded: ")).append(path), 0);
		}
	}

	openFileDialog->Close();
}

void MyFrame::OnSaveFile(wxCommandEvent& WXUNUSED(event))
{
	if(!m_Package.FlushChanges())
	{
		wxMessageBox(_("There was an error in saving the package file!"),
					 _("Save Error"), wxOK | wxICON_ERROR);
		SetStatusText(_("Failed to save the package!"), 0);
		return;
	}

	SetStatusText(_("Successfully saved the package!"), 0);
}

void MyFrame::OnSaveAsFile(wxCommandEvent& WXUNUSED(event))
{
}
