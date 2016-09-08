//--------------------------------------------------------------------------//
// Little resolution selector dialog by Raizor                              //
// ------------------------------------                                     //
// Still some work to do as aspect ratio is always 4:3 or 16:9 atm          //
// Need to add support for more resolutions too...                          //
//--------------------------------------------------------------------------//

#include "../resSelector.h"
#include "../../resources/resource.h"

SCREEN_INFO globalScreenInfo;

//
//  Message handler for resolution dialog box
//
BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG: // dialog initialized
			{
				// dialog init - populate combo box with resolutions
				HWND hwndCombo = GetDlgItem(hwnd, IDC_COMBO_RES);
				// current res
				int currentWidth = GetSystemMetrics(SM_CXSCREEN);
				int currentHeight = GetSystemMetrics(SM_CYSCREEN);

				// current bpp (bits per pixel)
				HDC screenDC = GetDC(NULL);
				int bpp = GetDeviceCaps(screenDC,BITSPIXEL);
				ReleaseDC(NULL,screenDC);

				int idx = 0;

                /*
				if (currentWidth == 800 && currentHeight == 600) idx = 1;
				if (currentWidth == 1024 && currentHeight == 768) idx = 2;
				if (currentWidth == 1152 && currentHeight == 864) idx = 3;
				if (currentWidth == 1200 && currentHeight == 800) idx = 4;
				if (currentWidth == 1280 && currentHeight == 720) idx = 5;
				if (currentWidth == 1360 && currentHeight == 768) idx = 6;
				if (currentWidth == 1600 && currentHeight == 900) idx = 7;
				if (currentWidth == 1680 && currentHeight == 1050) idx = 8;
				if (currentWidth == 1920 && currentHeight == 1080) idx = 9;
				if (currentWidth == 1920 && currentHeight == 1200) idx = 10;*/

				// set widescreen checkbox as ticked
				if (idx > 5)
				{
					//SendMessage(GetDlgItem(hwnd, IDC_CHECK_WS), BM_SETCHECK, 1, 0);
				}

				// always default to fullscreen - tick fs box
				SendMessage(GetDlgItem(hwnd, IDC_CHECK_FS), BM_SETCHECK, 1, 0);

				// always post fx
				SendMessage(GetDlgItem(hwnd, IDC_CHECK_NOISE), BM_SETCHECK, 1, 0);

				// populate combo with list of resolutions
				// we auto select the current res if it's in the list
                char txt1 [100];
                sprintf(txt1, "%d x %d x 32", GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT(txt1)); // 4:3				
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("800 x 600 x 32"));// 4:3				
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1024 x 768 x 32")); // 4:3
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1152 x 864 x 32")); // 4:3 
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1200 x 800 x 32"));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1280 x 720 x 32"));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1360 x 768 x 32"));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1600 x 900 x 32"));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1680 x 1050 x 32"));				
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1920 x 1080 x 32"));
				SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("1920 x 1200 x 32"));	

				// auto select item 0 (current res), event is picked up in message loop and 
				// widescreen checkbox is auto-selected if chosen res is widescreen.
				// only 16:9 atm tho...
				SendMessage(hwndCombo, CB_SETCURSEL, idx, 0);

				// populate fbo sizes
				HWND hwndComboFbo = GetDlgItem(hwnd, IDC_COMBO_FB);
				SendMessage(hwndComboFbo, CB_ADDSTRING, 0, (LPARAM)TEXT("Screen")); 		
				SendMessage(hwndComboFbo, CB_ADDSTRING, 0, (LPARAM)TEXT("256x256")); 
				SendMessage(hwndComboFbo, CB_ADDSTRING, 0, (LPARAM)TEXT("512x512")); 
				SendMessage(hwndComboFbo, CB_ADDSTRING, 0, (LPARAM)TEXT("1024x1024")); 

				SendMessage(hwndComboFbo, CB_SETCURSEL, 2, 0);
								
				/*
				// populate detail level
				HWND hwndComboDetail = GetDlgItem(hwnd, IDC_COMBO_DETAIL);
				SendMessage(hwndComboDetail, CB_ADDSTRING, 0, (LPARAM)TEXT("Low")); 		
				SendMessage(hwndComboDetail, CB_ADDSTRING, 0, (LPARAM)TEXT("Medium")); 
				SendMessage(hwndComboDetail, CB_ADDSTRING, 0, (LPARAM)TEXT("High")); 

				SendMessage(hwndComboDetail, CB_SETCURSEL, 1, 0);

				// populate shaders
				HWND hwndComboShaders = GetDlgItem(hwnd, IDC_COMBO_SHADERS);
				SendMessage(hwndComboShaders, CB_ADDSTRING, 0, (LPARAM)TEXT("None")); 
				SendMessage(hwndComboShaders, CB_ADDSTRING, 0, (LPARAM)TEXT("No Post-FX")); 
				SendMessage(hwndComboShaders, CB_ADDSTRING, 0, (LPARAM)TEXT("All")); 			
				

				SendMessage(hwndComboShaders, CB_SETCURSEL, 2, 0);	*/	

				return TRUE;
			}
        
        case WM_COMMAND: // button command
			
            switch(LOWORD(wParam)) // LOWORD of wParam is the control ID
            {
				case IDC_COMBO_RES: // res combo box
				{
					if (HIWORD(wParam) == CBN_SELCHANGE)
					{
						int resIndex = SendMessage(GetDlgItem(hwnd, IDC_COMBO_RES), CB_GETCURSEL, 0, 0);

						// set widescreen flag
						if (resIndex > 5)
						{					
							//SendMessage(GetDlgItem(hwnd, IDC_CHECK_WS), BM_SETCHECK, 1, 0); // yes
						}else{
							//SendMessage(GetDlgItem(hwnd, IDC_CHECK_WS), BM_SETCHECK, 0, 0); // no
						}
					}
					break;
				}

                case IDOK: // ok button
					{						
						// probe choices for checkboxes
						globalScreenInfo.fullscreen = SendMessage(GetDlgItem(hwnd, IDC_CHECK_FS), BM_GETCHECK, 0, 0); // fullscreen checked?
                        globalScreenInfo.addNoise = SendMessage(GetDlgItem(hwnd, IDC_CHECK_NOISE), BM_GETCHECK, 0, 0); // fullscreen checked?
                        globalScreenInfo.prerenderAudio = SendMessage(GetDlgItem(hwnd, IDC_CHECK_RENDERAUDIO), BM_GETCHECK, 0, 0); // fullscreen checked?
						//globalScreenInfo.widescreen = SendMessage(GetDlgItem(hwnd, IDC_CHECK_WS), BM_GETCHECK, 0, 0); // widescreen checked?
						//globalScreenInfo.postFx = SendMessage(GetDlgItem(hwnd, IDC_CHECK_PP), BM_GETCHECK, 0, 0); // widescreen checked?
						int resIndex = SendMessage(GetDlgItem(hwnd, IDC_COMBO_RES), CB_GETCURSEL, 0, 0);

						switch(resIndex) // set res according to selection
						{
							case 0: { globalScreenInfo.width = GetSystemMetrics(SM_CXSCREEN); globalScreenInfo.height = GetSystemMetrics(SM_CYSCREEN); break; }
							case 1: { globalScreenInfo.width = 800; globalScreenInfo.height = 600; break; }
							case 2: { globalScreenInfo.width = 1024; globalScreenInfo.height = 768; break; }
							case 3: { globalScreenInfo.width = 1152; globalScreenInfo.height = 864; break; }
							case 4: { globalScreenInfo.width = 1200; globalScreenInfo.height = 800; break; }
							case 5: { globalScreenInfo.width = 1280; globalScreenInfo.height = 720; break; }
							case 6: { globalScreenInfo.width = 1360; globalScreenInfo.height = 768; break; }
							case 7: { globalScreenInfo.width = 1600; globalScreenInfo.height = 900; break; }
							case 8: { globalScreenInfo.width = 1680; globalScreenInfo.height = 1050; break; }
							case 9: { globalScreenInfo.width = 1920; globalScreenInfo.height = 1080; break; }
							case 10: { globalScreenInfo.width = 1920; globalScreenInfo.height = 1200; break; }
						}
                        /*
						// set fbo size
						resIndex = SendMessage(GetDlgItem(hwnd, IDC_COMBO_FB), CB_GETCURSEL, 0, 0);
						switch(resIndex)
						{
						case 0: 
							{ 
								globalScreenInfo.fboWidth = globalScreenInfo.width; 
								globalScreenInfo.fboHeight = globalScreenInfo.height; 
								break; 
							}
						case 1: 
							{ 
								globalScreenInfo.fboWidth = 256; 
								globalScreenInfo.fboHeight = 256; 
								break; 
							}
						case 2: 
							{ 
								globalScreenInfo.fboWidth = 512; 
								globalScreenInfo.fboHeight = 512; 
								break; 
							}
						case 3: 
							{ 
								globalScreenInfo.fboWidth = 1024; 
								globalScreenInfo.fboHeight = 1024; 
								break; 
							}
						}*/

						// close dialog
						EndDialog(hwnd, IDOK);						
						break;
					}
                case IDCANCEL:
					{
						// cancel button - close dialog and quit
						EndDialog(hwnd, IDCANCEL);
						PostQuitMessage(0);
                        globalScreenInfo.quitApp = true;
						break;
					}
                
            }
        break;
        default:
            return FALSE;
    }
    return TRUE;
}

int SelectResolution(HINSTANCE hinstance)
{
	return DialogBox(hinstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, AboutDlgProc);    
}