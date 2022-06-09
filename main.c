#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <vlc/vlc.h>
#include <signal.h>

#include "libs/run_command.h"

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


char FileList[30][100] = {"Blue_Bird.mp4","Sign.mp4","Lovers.mp4","Diver.mp4","Moshimo.mp4","Not_Even_Sudden_Rain_Can_Defeat_Me.mp4","Tsuki_no_Ookisa.mp4","Crimson_Lotus.mp4","Hotaru_no_Hikari.mp4", "Closer.mp4"};
int lengthFileList = 10;


libvlc_instance_t * instance;
libvlc_media_t *media;
libvlc_media_player_t *media_player;
int posPlaylist = 0;
int snapShotExist = 0;
int snapShotCreatedOnce = 0;
int stateToChangeToPlayPause = 1;
char* musicName = "";
GtkWidget *window;
GtkWidget *label_info_music;
GtkWidget *snapshotWidget;

int countNumberFileInList() {
    int temp;
    int iTemp = 0; 
    while (FileList[iTemp]){
        temp++;
        iTemp++;
    }
    return temp;
    
}


void intHandler(int dummy) 
{
    exit(1);   // Do whatever you want here to handle it...
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
    printf("Child %u finished with status %d\n", child_pid, child_status);*/
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

static void on_response_file_chooser(GtkDialog *dialog,int response){
    if (response == GTK_RESPONSE_ACCEPT)
    {
      GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
      g_autoptr(GFile) file = gtk_file_chooser_get_file(chooser); // TODO: change to use gtk_file_chooser_get_files()
      // add to playlist the musics
    }

    gtk_window_destroy(GTK_WINDOW(dialog));
}


static void openFileChooser(GtkWidget *widget, gpointer data){
    GtkWidget *file_chooser_dialog = gtk_file_chooser_dialog_new("Choose files", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN, "Choose");


    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_dialog_new ("Open File",GTK_WINDOW(window),action, ("_Cancel"),GTK_RESPONSE_CANCEL, ("_Open"),GTK_RESPONSE_ACCEPT,NULL);
    gtk_widget_show(dialog);
    g_signal_connect(dialog, "response", G_CALLBACK (on_response_file_chooser),NULL);
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
    
}


static void stop(GtkWidget* widget, gpointer data){
        libvlc_media_player_stop(media_player);
}

static void next(GtkWidget* widget, gpointer data){
    if (posPlaylist < lengthFileList - 1){
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
}

static void previous(GtkWidget* widget, gpointer data){
    if (posPlaylist > 0){
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
}

/*static void setXWindow(GtkWidget *widget, gpointer data){
    libvlc_media_player_set_xwindow(media_player, gtk_application_window_get_id(gtk_widget_get_window(widget)));
}*/

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
    gtk_window_set_default_size(GTK_WINDOW(window),200,200);
    g_signal_connect(openFileButton, "clicked", G_CALLBACK(openFileChooser), NULL);
    g_signal_connect(button_previous, "clicked", G_CALLBACK(previous), NULL);
    g_signal_connect(button_play, "clicked", G_CALLBACK(play), NULL);
    g_signal_connect(button_stop, "clicked", G_CALLBACK(stop), NULL);
    g_signal_connect(button_next, "clicked", G_CALLBACK(next), NULL);
    gtk_window_set_child(GTK_WINDOW(window), mainUIBox);
    gtk_popover_set_child(GTK_POPOVER(popover), menuBox);
    gtk_box_append(GTK_BOX(menuBox), openFileButton);
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
    //lengthFileList = countNumberFileInList();
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