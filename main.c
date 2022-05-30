#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <vlc/vlc.h>
#include <signal.h>

#include "libs/run_command.h"

//int countNumberFileInList();

char FileList[30][100] = {"Blue_Bird.mp4","Sign.mp4","Lovers.mp4","Diver.mp4","Moshimo.mp4","Not_Even_Sudden_Rain_Can_Defeat_Me.mp4","Tsuki_no_Ookisa.mp4","Crimson_Lotus.mp4","Hotaru_no_Hikari.mp4", "Closer.mp4"};
int lengthFileList = 10;


libvlc_instance_t * instance;
libvlc_media_t *media;
libvlc_media_player_t *media_player;
int posPlaylist = 0;
int snapShotExist = 0;
int stateToChangeToPlayPause = 1;
char* musicName = "";
GtkWidget *label_info_music;
GtkWidget *snapshotWidget;

/*int countNumberFileInList() {
    int temp;
    int iTemp = 0; 
    while (FileList[iTemp] != "" && FileList[iTemp] != 0){
        temp++;
        iTemp++;
    }
    return temp;
    
}*/


void intHandler(int dummy) 
{
    exit(1);   // Do whatever you want here to handle it...
}


void updateMusicInfo(){
    /*char tempCommand[100];
    snprintf(tempCommand, sizeof(tempCommand),"ffmpeg -i %s -r 1 -frames:v 1 -s 1280x720 -f image2 snapshot-audio-player.jpg -y", FileList[posPlaylist]);
    system(tempCommand);
    snapShotExist = 1;
    snapshotWidget = gtk_image_new_from_file("snapshot-audio-player.jpg");*/
    char temp[100];
    snprintf(temp,100, "%s", FileList[posPlaylist]);
    gtk_label_set_text(GTK_LABEL(label_info_music), temp);
}

static void play(GtkWidget *widget, gpointer data){
    signal(SIGINT, intHandler); 
    signal(SIGTERM, intHandler); 
    signal(SIGQUIT, intHandler); 
    signal(SIGKILL, intHandler);
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
    GtkWidget *window = gtk_application_window_new(app);
    snapshotWidget = gtk_image_new();
    //GtkWidget *player_widget;
    GtkWidget *button_play  = gtk_button_new_with_label("Play");
    GtkWidget *button_stop  = gtk_button_new_with_label("Stop");
    GtkWidget *button_next  = gtk_button_new_with_label("Next");
    GtkWidget *button_previous  = gtk_button_new_with_label("Previous");
    GtkWidget *label_title= gtk_label_new("Audio Player");
    label_info_music = gtk_label_new("");
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
    g_signal_connect(button_previous, "clicked", G_CALLBACK(previous), NULL);
    g_signal_connect(button_play, "clicked", G_CALLBACK(play), NULL);
    g_signal_connect(button_stop, "clicked", G_CALLBACK(stop), NULL);
    g_signal_connect(button_next, "clicked", G_CALLBACK(next), NULL);
    gtk_window_set_child(GTK_WINDOW(window), mainUIBox);
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