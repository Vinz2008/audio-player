#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk/x11/gdkx.h>
/*#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-x11.h>*/
#include <vlc/vlc.h>
#include <signal.h>

#include "libs/array.h"

//int countNumberFileInList();


#ifdef _WIN32

#include <windows.h>
#include <tchar.h>

int windows_system(const char *cmd)
{
  PROCESS_INFORMATION p_info;
  STARTUPINFO s_info;
  LPSTR cmdline, programpath;

  memset(&s_info, 0, sizeof(s_info));
  memset(&p_info, 0, sizeof(p_info));
  s_info.cb = sizeof(s_info);

  cmdline = _tcsdup(TEXT(cmd));
  programpath = _tcsdup(TEXT(cmd));

  if (CreateProcess(programpath, cmdline, NULL, NULL, 0, 0, NULL, NULL, &s_info, &p_info))
  {
    WaitForSingleObject(p_info.hProcess, INFINITE);
    CloseHandle(p_info.hProcess);
    CloseHandle(p_info.hThread);
  }
}
#endif


//char FileList[][100] = {"Blue_Bird.mp4","Sign.mp4","Lovers.mp4","Diver.mp4","Moshimo.mp4","Not_Even_Sudden_Rain_Can_Defeat_Me.mp4","Tsuki_no_Ookisa.mp4","Crimson_Lotus.mp4","Hotaru_no_Hikari.mp4", "Closer.mp4"};
char** FileList;
char** FileListTest;
int lengthFileListTest = 0;

int musicStarted = 0;

int lengthFileList = 0;
//int lengthFileList = 10;

libvlc_instance_t* instance;
libvlc_instance_t* instance_video;
libvlc_media_t *media;
libvlc_media_t *media_video;
libvlc_media_player_t *media_player;
libvlc_media_player_t *media_player_video;
int videoMode = 0;
int videoModeInstanceCreated = 0;
int posPlaylist = 0;
int snapShotExist = 0;
int snapShotCreatedOnce = 0;
int stateToChangeToPlayPause = 1;
char* musicName = "";
GtkWidget *window;
GtkWidget* player_widget;
GtkWidget *label_info_music;
GtkWidget *snapshotWidget;
GtkWidget* videoWindow;

size_t countNumberFileInList(char* list[100]) {
    size_t len = 0; 
    /*printf("sizeof list %lu\n", sizeof((*list)));
    printf("sizeof char %lu\n", sizeof(char));
    printf("sizeof char*  %lu\n", sizeof(char*));
    len = sizeof(list)/(sizeof(char*));
    printf("len : %lu\n", len);*/
    while (list[len] != "\0"){
        printf("len : %lu\n", len);
        len++;
    }
    return len;
    
}


void intHandler(int dummy) 
{
    exit(1);   // Do whatever you want here to handle it...
}

void player_widget_on_realize(GtkWidget *widget, gpointer data){
    //libvlc_media_player_set_xwindow(media_player_video, gtk_application_window_get_id(GTK_WINDOW(player_widget)));
    GdkSurface* surface = gtk_native_get_surface(gtk_widget_get_native(player_widget));
    //gdk_x11_surface_get_xid(surface);
    libvlc_media_player_set_xwindow(media_player_video, (uint32_t)gdk_x11_surface_get_xid(surface));
    printf("X window assigned\n");
    printf("X window assigned\n");
    printf("X window assigned\n");
    printf("X window assigned\n");
    //libvlc_media_player_set_xwindow(media_player, GDK_WINDOW_XID(gdk_x11_surface_get_xid(surface)));
    //libvlc_media_player_set_xwindow(media_player, GDK_WINDOW_XID(gtk_widget_get_window(widget)));
}


void updateMusicInfo(){
    /*pid_t pid = vfork();
    if(pid == -1)
    {
    printf("fork() failed");
    exit(1);
    }
    else if(pid == 0)
    {
    char *argsArray[2] = { NULL };
    printf("test\n");
    strcpy(argsArray[0], "/usr/bin/ffmpeg");
    printf("test2\n");
    snprintf(argsArray[1], sizeof(argsArray[1]), "-ss 00:00:02 -i %s -frames:v 1 -f image2 snapshot-audio-player.jpg -y", FileList[posPlaylist]);
    strcpy(argsArray[2], (char *)0);
    execv(argsArray[0], argsArray);
    }
    int child_status;
    int child_pid = wait(&child_status);
    printf("Child %u finished with status %d\n", child_pidGtk, child_status);*/
    remove("snapshot-audio-player.jpg");
    char tempCommand[110];
    snprintf(tempCommand, sizeof(tempCommand),"ffmpeg -ss 00:00:02 -i %s -frames:v 1 -f image2 snapshot-audio-player.jpg -y", FileList[posPlaylist]);
    #ifndef _WIN32
    system(tempCommand);
    #else
    windows_system(tempCommand);
    #endif
    //sleep(3);

    snapShotExist = 1;
    //gtk_image_clear(GTK_IMAGE(snapshotWidget));
    if (snapShotCreatedOnce == 1){
        gtk_image_set_from_file(GTK_IMAGE(snapshotWidget), "snapshot-audio-player.jpg");
    } else {
    snapshotWidget = gtk_image_new_from_file("snapshot-audio-player.jpg");
    }
    snapShotCreatedOnce = 1;
    char temp[100];
    snprintf(temp,100, "%s", FileList[posPlaylist]);
    gtk_label_set_text(GTK_LABEL(label_info_music), temp);
}


void on_key_press(GtkWidget* widget, GdkEvent* event){
    /*struct xkb_context *ctx;
    ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!ctx){
        printf("Couldn't create xkbcommon context\n");
        exit(1);
    }
    struct xkb_keymap* keymap;
    xcb_connection_t* conn = xcb_connect(NULL, NULL);
    int32_t device_id;
    device_id = xkb_x11_get_core_keyboard_device_id(conn);
    if (device_id == -1){
        printf("ERROR : Can't get device id\n");
        exit(1);
    }
    keymap = xkb_x11_keymap_new_from_device(ctx, conn, device_id, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap){
        printf("ERROR : Can't get keymap\n");
        exit(1);
    }
    struct xkb_state *state;
    state = xkb_x11_state_new_from_device(keymap, conn, device_id);
    if (!state){
        printf("ERROR : Can't get xkb state\n");
        exit(1);
    }
    xkb_keycode_t keycode;
    xkb_keysym_t keysym;
    keycode = gdk_key_event_get_keycode(event);
    keysym = xkb_state_key_get_one_sym(state, keycode);
    if (keysym == XKB_KEY_Escape){
        printf("ESCAPE PRESSED\n");
        libvlc_media_player_stop(media_player_video);
        libvlc_media_player_release(media_player);
        libvlc_release(instance); 
    }*/
}


void videoWindow_on_delete(GtkWidget *widget, gpointer data){
    libvlc_media_player_release(media_player_video);
    libvlc_release(instance_video); 
    gtk_window_close(GTK_WINDOW(videoWindow));
    gtk_window_destroy(GTK_WINDOW(videoWindow));
}


static void changeVideoMode(GtkWidget *widget, gpointer data){
    if (videoMode == 0){
        //if (videoModeInstanceCreated == 0){
            //const char* argvVlc[] = { "--no-video" };1
            if (musicStarted == 1){
            libvlc_media_player_stop(media_player);
            }
            instance_video = libvlc_new(0, NULL);
            media_player_video = libvlc_media_player_new(instance_video);
#ifndef _WIN32
            signal(SIGINT, intHandler); 
            signal(SIGTERM, intHandler); 
            signal(SIGQUIT, intHandler); 
            signal(SIGKILL, intHandler);
#endif
            media_video = libvlc_media_new_path(instance_video, FileList[posPlaylist]);
            media_player_video = libvlc_media_player_new_from_media(media_video);
            printf("added video mode\n");
            videoWindow = gtk_window_new();
            gtk_window_set_title(GTK_WINDOW(videoWindow), "Video Window");
            player_widget = gtk_drawing_area_new();
            GtkWidget *videoMainUIBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
            gtk_window_set_child(GTK_WINDOW(videoWindow), videoMainUIBox);
            gtk_box_append(GTK_BOX(videoMainUIBox), player_widget);
            gtk_window_present(GTK_WINDOW(videoWindow));
            g_signal_connect(G_OBJECT(videoWindow), "delete-event", G_CALLBACK(videoWindow_on_delete), NULL);
            //g_signal_connect(G_OBJECT(player_widget), "realize", G_CALLBACK(player_widget_on_realize), NULL);
            player_widget_on_realize(widget, NULL);
            //g_signal_connect(player_widget, "key-press-event", G_CALLBACK(on_key_press), NULL);
            
            /*GdkEvent *event;
            //GTYPE* eventType = gdk_key_event_get_type();
            GdkSurface* surface = gtk_native_get_surface(GTK_NATIVE(player_widget));
            GdkDisplay* display = gdk_surface_get_display(surface);*/


            //GdkEventType* eventType;
            libvlc_video_set_key_input(media_player_video, false);
            libvlc_media_player_play(media_player_video);
            printf("video started\n");
            
        /*} else {
            libvlc_media_player_stop(media_player);
            media_player_video = libvlc_media_player_new(instance_video);


        }*/
        videoMode = 1;
    } else {
        libvlc_media_player_release(media_player_video);
        libvlc_release(instance_video); 
        gtk_window_close(GTK_WINDOW(videoWindow));
        gtk_window_destroy(GTK_WINDOW(videoWindow));
        videoMode = 0;
        
    }

}

static void on_response_file_chooser(GtkDialog *dialog,int response){
    if (response == GTK_RESPONSE_ACCEPT)
    {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
      
      g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser); // TODO: change to use gtk_file_chooser_get_files()
      char* filename = g_file_get_path(file);
      /*gtk_file_chooser_set_select_multiple(chooser, TRUE);
      g_autoptr(GListModel) file =  gtk_file_chooser_get_files(chooser);*/
      //append_to_array(filename, FileListTest);
      //FileListTest[lengthFileListTest] = filename;
      FileList = append_to_array(filename, FileList);
      printf("out1\n");
      lengthFileList++;
      printf("out2\n");
      for (int i = 0; i < lengthFileList; i++){
        printf("out?\n");
        printf("FileList[%d] :  %s\n",i, FileList);
      }
      // add to playlist the musics
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}


static void openFileChooser(GtkWidget *widget, gpointer data){
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_dialog_new("Open File",GTK_WINDOW(window),action, ("_Cancel"),GTK_RESPONSE_CANCEL, ("_Open"),GTK_RESPONSE_ACCEPT,NULL);
    gtk_widget_show(dialog);
    g_signal_connect(dialog, "response", G_CALLBACK(on_response_file_chooser),NULL);
}

static void play(GtkWidget *widget, gpointer data){
#ifndef _WIN32
    signal(SIGINT, intHandler); 
    signal(SIGTERM, intHandler); 
    signal(SIGQUIT, intHandler); 
    signal(SIGKILL, intHandler);
#endif
    media = libvlc_media_new_path(instance, FileList[posPlaylist]);
    media_player = libvlc_media_player_new_from_media(media);
    printf("is playing : %i\n", libvlc_media_player_is_playing(media_player));
    if (libvlc_media_player_is_playing(media_player) == true){
        libvlc_media_player_pause(media_player);
        printf("music paused\n");
    } else {
        if (libvlc_media_player_is_playing(media_player) == false){
    libvlc_media_player_play(media_player);
    printf("music started\n");
        }
    }
    musicStarted = 1;
    
}


static void stop(GtkWidget* widget, gpointer data){
    libvlc_media_player_stop(media_player);
    musicStarted = 0;
}

static void next(GtkWidget* widget, gpointer data){
    if (posPlaylist <= lengthFileList - 1){
        posPlaylist++;
    } else {
        posPlaylist = 0;
    }
    updateMusicInfo();
    printf("filename : %s\n", FileList[posPlaylist]);
    if (libvlc_media_player_is_playing(media_player) == true){
    libvlc_media_player_stop(media_player);
    }
    media = libvlc_media_new_path(instance, FileList[posPlaylist]);
    media_player = libvlc_media_player_new_from_media(media);
    libvlc_media_player_play(media_player);
    musicStarted = 1;
}

static void previous(GtkWidget* widget, gpointer data){
    if (posPlaylist >= 0){
        posPlaylist--;
    } else {
        posPlaylist = lengthFileList - 1;
    }
    updateMusicInfo();
     printf("filename : %s\n", FileList[posPlaylist]);
    if (libvlc_media_player_is_playing(media_player) == true){
    libvlc_media_player_stop(media_player);
    }
    media = libvlc_media_new_path(instance, FileList[posPlaylist]);
    media_player = libvlc_media_player_new_from_media(media);
    libvlc_media_player_play(media_player);
    musicStarted = 1;
}

static void setXWindow(GtkWidget *widget, gpointer data){
    //libvlc_media_player_set_xwindow(media_player, gtk_application_window_get_id(gtk_widget_get_window(widget)));
}

static void activate(GtkApplication *app, gpointer vlc_structure){
    window = gtk_application_window_new(app);
    snapshotWidget = gtk_image_new();
    //GtkWidget *player_widget;
    GtkWidget *button_play  = gtk_button_new_with_label("Play");
    GtkWidget *button_stop  = gtk_button_new_with_label("Stop");
    GtkWidget *button_next  = gtk_button_new_with_label("Next");
    GtkWidget *button_previous  = gtk_button_new_with_label("Previous");
    GtkWidget *button_menu = gtk_menu_button_new();
    GtkWidget* popover = gtk_popover_new();
    GtkWidget* menuBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    GtkWidget* openFileButton = gtk_button_new_with_label("Open file");
    GtkWidget* videoModeButton = gtk_button_new_with_label("Add video mode");
    GtkWidget *label_title= gtk_label_new("Audio Player");
    label_info_music = gtk_label_new("");
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(button_menu), "open-menu");
    gtk_menu_button_set_popover(GTK_MENU_BUTTON(button_menu), popover), 
    gtk_widget_set_halign(button_menu, GTK_ALIGN_END);
    gtk_widget_set_valign(button_menu, GTK_ALIGN_END);
    //player_widget = gtk_drawing_area_new();
    //g_signal_connect(G_OBJECT(player_widget), "realize", G_CALLBACK(setXWindow), NULL);
    updateMusicInfo();
    GtkWidget *frame = gtk_frame_new(NULL);
    GtkWidget *mainUIBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
    GtkWidget *buttonsBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
    gtk_widget_set_margin_bottom(buttonsBox, 4);
    gtk_widget_set_halign(buttonsBox, GTK_ALIGN_CENTER);
    gtk_box_set_homogeneous(GTK_BOX(mainUIBox), TRUE);
    gtk_window_set_title(GTK_WINDOW(window), "Audio Player");
    gtk_window_set_default_size(GTK_WINDOW(window),355,200);
    g_signal_connect(openFileButton, "clicked", G_CALLBACK(openFileChooser), NULL);
    g_signal_connect(videoModeButton, "clicked", G_CALLBACK(changeVideoMode), NULL);
    g_signal_connect(button_previous, "clicked", G_CALLBACK(previous), NULL);
    g_signal_connect(button_play, "clicked", G_CALLBACK(play), NULL);
    g_signal_connect(button_stop, "clicked", G_CALLBACK(stop), NULL);
    g_signal_connect(button_next, "clicked", G_CALLBACK(next), NULL);
    gtk_window_set_child(GTK_WINDOW(window), mainUIBox);
    gtk_popover_set_child(GTK_POPOVER(popover), menuBox);
    gtk_box_append(GTK_BOX(menuBox), openFileButton);
    gtk_box_append(GTK_BOX(menuBox), videoModeButton);
    gtk_box_append(GTK_BOX(mainUIBox), button_menu);
    gtk_box_append(GTK_BOX(mainUIBox), label_title);
    gtk_box_append(GTK_BOX(mainUIBox), snapshotWidget);
    //gtk_box_append(GTK_BOX(mainUIBox), player_widget);
    gtk_box_append(GTK_BOX(mainUIBox), label_info_music);
    gtk_box_append(GTK_BOX(mainUIBox), buttonsBox);
    gtk_box_append(GTK_BOX(buttonsBox), button_previous);
    gtk_box_append(GTK_BOX(buttonsBox), button_play);
    gtk_box_append(GTK_BOX(buttonsBox), button_stop);
    gtk_box_append(GTK_BOX(buttonsBox), button_next);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv){
    //lengthFileList = countNumberFileInList(fileList);
    //FileListTest = malloc(3 * sizeof(char*));
    /*FileListTest = (char**)malloc(3 * sizeof(char*));
    FileListTest[0] = malloc(strlen("hello") * sizeof(char));
    strcpy(FileListTest[0], "hello");
    FileListTest[1] = malloc(strlen("world") * sizeof(char));
    strcpy(FileListTest[1], "world");
    FileListTest =  append_to_array("github", FileListTest);*/
    FileList = malloc(1 * sizeof(char*));
    FileList[0] = malloc(strlen("Blue_Bird.mp4") * sizeof(char));
    lengthFileList++;
    strcpy(FileList[0], "Blue_Bird.mp4");
    instance = libvlc_new(0, NULL);
    const char* argvVlc[] = { "--no-video" };
    instance = libvlc_new( 1, argvVlc );
    media_player = libvlc_media_player_new(instance);
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.audio-player.audio-player", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    libvlc_media_player_release(media_player);
    libvlc_release(instance); 
    return status;

}