/**
 * @file main.c
 * @author LeKiwiDeBx
 * @brief
 * @version 0.1
 * @date 2024-05-02
 *
 * @copyright Copyright (c) 2024
 *
 */
#include </usr/include/ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * headers GTK/Glib
 */
#include </usr/include/glib-2.0/glib.h>
#include <gtk-4.0/gtk/gtk.h>
#include <glib/gprintf.h>
#include <math.h>

/**
 * translation
 */
#include <glib/gi18n.h>
#include <libintl.h>
#include <locale.h>

#include "main.h"

#ifndef _LINUX_
#define _LINUX_ /* execution de system clear */
#endif

#define GETTEXT_DOMAIN_NAME "poflash"
#define GETTEXT_DIR_NAME "locale"
#define POFLASH_LOCALE_FILE "fr_FR.UTF-8"
#define APPLICATION_TITLE "PoFlash"
#define POFLASH_THEME_FILE "custom.css"
#define POFLASH_THEME_PATH "theme/css/"
#define POFLASH_PICTURE_DRAG_AND_DROP "image/glisser-deposer.png"

#define G_VALUE_FILE_BASENAME(value) (g_file_get_basename(G_FILE(g_value_get_object(value))))
#define G_VALUE_FILE_PARSE_NAME(value) (g_file_get_parse_name(G_FILE(g_value_get_object(value))))
#define G_VALUE_FILE_PATH(value) (g_file_get_path(g_file_get_parent(G_FILE(g_value_get_object(value)))))
#define GTK_GET_TEXT_ENTRY(textentry) (gtk_entry_buffer_get_text(gtk_entry_get_buffer(GTK_ENTRY(textentry))))

#define UTF8 "UTF-8"
#define INPUT_FILE_CURRENT_DIR "./*"
#define PACKAGE_VERSION_DEFAULT "1.0"
#define COPYRIGHT_HOLDER_DEFAULT (getlogin() ? getlogin() : "")
#define PACKAGE_NAME_PLACEHOLDER _("Your Package Name")
#define MSG_ID_BUGS_ADDRESS_PLACEHOLDER _("Email@Address")
#define OUTPUT_DIR_DEFAULT "locale"
#define ADD_COMMENTS_DEFAULT "TRANSLATORS"

/* application instance */
GtkApplication *app;
/* fenetre principal application */
GtkWidget *pWindowMain;
/* boite de dialogue pour la fermeture */
GtkWidget *pDialogBoxQuit;
/* recept drag and drop */
GtkWidget *pImageDragAndDrop;
/* css provider */
GtkCssProvider *pCssProvider;
/* entry for keyword */
GtkWidget *pEntryKeyword;
/* entry for package name*/
GtkWidget *pEntryPackageName;
/* entry for msgid-bugs-address*/
GtkWidget *pEntryMsgIdBugsAddress;
/* view for line command xgettext*/
GtkWidget *pTextViewLineCommand;
/* buffer for line command xgettext*/
GtkTextBuffer *pBufferTextViewLineCommand;
/**
 * @brief Appel pour fin d'application
 * @param pWidget appele par les boutons quit et fermeture fenetre
 * @param pData NULL
 */
static void OnDestroy(GtkWidget *pWidget, gpointer pData);

/**
 * @brief Appel pour fermeture fenetre
 *
 * @param pWidget est la boite de dialogue pour la fermeture
 * @param response_id est la valeur de la reponse du bouton
 * @param data NULL
 */
static void OnResponse(GtkWidget *pWidget, gint response_id, gpointer data);

/**
 * @brief Appel pour lancer xgettext sur les fichiers deposes
 *
 * @param pWidget Button xgettext
 * @param data
 */
static void OnClickXgettext(GtkWidget *pWidget, gpointer data);

/**
 * @brief  active le switch pour l'option --from-code
 *
 * @param pWidget self
 * @param state etat actif/inactif
 * @param data pXgettext
 * @return gboolean
 */
static gboolean OnSwitchFromCode(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option --force-po
 *
 * @param pWidget self
 * @param state etat actif/inactif
 * @param data pXgettext
 * @return gboolean
 */
static gboolean OnSwitchForcePo(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option --package-version
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchPackageVersion(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option --copyright-holder
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchCopyrightHolder(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option --c++ shorthand for --language=C++
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchCplusplus(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option -p output files will be placed in directory DIR
 *
 * @param pWidget
 * @param state
 * @param data
 */
static gboolean OnSwitchOutputDir(GtkWidget *pWidget, gboolean state, gpointer data);
/**
 * @brief   active le switch pour l'option -c --add-comments=TAG place comment block with TAG
 * (or those preceding keyword lines) in output file
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchAddComments(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief Devine le nom du package dans le repertoire en se basant sur le nom d'un fichier compilé type data)
 *
 * @param pDir
 * @return gchar*
 */

static gchar *__getPackageGuessIntoDir(const gchar *pDir);
#define PACKAGE_NAME_DEFAULT __getPackageGuessIntoDir(".")
#define FILE_EXTENSION_POT ".pot"
#define OUTPUT_DEFAULT g_strconcat(PACKAGE_NAME_DEFAULT, FILE_EXTENSION_POT, NULL)

/**
 * @brief  active le switch pour deviner pour l'option --package-name=PACKAGE set package name in output
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchPackageGuess(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief active le switch pour l'option -o write output to specified file .po or .pot
 *
 * @param pWidget
 * @param state
 * @param data
 * @return g_boolean
 */
static gboolean OnSwitchOutput(GtkWidget *pWidget, gboolean state, gpointer data);

/**
 * @brief load css
 *
 */
static void __poflashLoadCss();

/**
 * @brief OnDrop sur l´image pour enregistrer un fichier ou des fichiers dans pXgettext
 *
 * @param target
 * @param value
 * @param x
 * @param y
 * @param data
 * @return gboolean
 */
static gboolean OnDrop(GtkDropTarget *target,
                       const GValue *value,
                       double x,
                       double y,
                       gpointer data);

/**
 * @brief build de l'application a la creation
 *
 * @param app
 * @param user_data
 */
static void
activate(GtkApplication *app,
         gpointer user_data)
{
    xgettext_args pXgettext = g_malloc(sizeof(struct s_xgettext_args));

    g_printf("START pXgettext->inputfile:|%p|\n", pXgettext->inputfile);
    pXgettext->inputfile = g_strdup(INPUT_FILE_CURRENT_DIR);
    pXgettext->from_code = g_strdup("");
    pXgettext->force_po = g_strdup("");
    pXgettext->keyword = g_strdup("");
    pXgettext->package_version = g_strdup("");
    pXgettext->package_name = g_strdup("");
    pXgettext->copyright_holder = g_strdup("");
    pXgettext->msgid_bugs_address = g_strdup("");
    pXgettext->cplusplus = g_strdup("");
    pXgettext->output_dir = g_strdup("");
    pXgettext->add_comments = g_strdup("");
    pXgettext->output = g_strdup(OUTPUT_DEFAULT);

    pWindowMain = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(pWindowMain), APPLICATION_TITLE);
    // gtk_window_set_default_size(GTK_WINDOW(pWindowMain), 640, 480);
    gtk_window_set_resizable(GTK_WINDOW(pWindowMain), TRUE);
    g_signal_connect(G_OBJECT(pWindowMain), "close-request", G_CALLBACK(OnDestroy), NULL);
    GtkWidget *pGridMain = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(pWindowMain), pGridMain);
    GdkPixbuf *pPixBufDragAndDrop = gdk_pixbuf_new_from_file_at_scale(POFLASH_PICTURE_DRAG_AND_DROP, 200, 200, TRUE, NULL);
    pImageDragAndDrop = gtk_image_new_from_pixbuf(pPixBufDragAndDrop);
    g_object_unref(pPixBufDragAndDrop);

    GtkDropTarget *target = gtk_drop_target_new(G_TYPE_INVALID, GDK_ACTION_COPY); // GDK_ACTION_ASK
    gtk_drop_target_set_gtypes(target, (GType[2]){
                                           G_TYPE_FILE,
                                           GDK_TYPE_PIXBUF,
                                       },
                               2);

    g_signal_connect(target, "drop", G_CALLBACK(OnDrop), pXgettext);
    gtk_widget_add_controller(GTK_WIDGET(pImageDragAndDrop), GTK_EVENT_CONTROLLER(target));
    gtk_widget_set_size_request(pImageDragAndDrop, 200, 200);
    gtk_grid_attach(GTK_GRID(pGridMain), pImageDragAndDrop, 0, 0, 1, 1);

    GtkWidget *pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *pBoxRowFromCode = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelFromCode = gtk_label_new(_("From code: UTF-8"));
    GtkWidget *pSwitchFromCode = gtk_switch_new();

    GtkWidget *pBoxRowForcePo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget *pBoxLabelForcePo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelForcePo = gtk_label_new(_("Force PO"));
    gtk_box_append(GTK_BOX(pBoxLabelForcePo), pLabelForcePo);
    gtk_widget_set_halign(GTK_WIDGET(pBoxLabelForcePo), GTK_ALIGN_START);

    GtkWidget *pBoxSwitchForcePo = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pSwitchForcePo = gtk_switch_new();
    gtk_box_append(GTK_BOX(pBoxSwitchForcePo), pSwitchForcePo);
    gtk_widget_set_halign(GTK_WIDGET(pBoxSwitchForcePo), GTK_ALIGN_END);

    GtkWidget *pBoxRowPackageVersion = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelPackageVersion = gtk_label_new(g_strconcat(_("Package version: "), PACKAGE_VERSION_DEFAULT, NULL));
    GtkWidget *pSwitchPackageVersion = gtk_switch_new();

    GtkWidget *pBoxRowOutput = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelOutput = gtk_label_new(g_strconcat(_("Output: "), OUTPUT_DEFAULT, NULL));
    GtkWidget *pSwitchOutput = gtk_switch_new();

    GtkWidget *pBoxRowCopyrightHolder = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelCopyrightHolder = gtk_label_new(g_strconcat(_("Copyright holder: "), COPYRIGHT_HOLDER_DEFAULT, NULL));
    GtkWidget *pSwitchCopyrightHolder = gtk_switch_new();

    GtkWidget *pBoxRowCplusplus = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelCplusplus = gtk_label_new(_("Language: C++ "));
    GtkWidget *pSwitchCplusplus = gtk_switch_new();

    GtkWidget *pBoxRowOutputDir = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelOutputDir = gtk_label_new(g_strconcat(_("Output dir: ./"), OUTPUT_DIR_DEFAULT, NULL));
    GtkWidget *pSwitchOutputDir = gtk_switch_new();

    GtkWidget *pBoxRowPackageName = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelPackageName = gtk_label_new(g_strconcat(_("Package name: "), PACKAGE_NAME_DEFAULT, NULL));
    GtkWidget *pSwitchPackageName = gtk_switch_new();

    GtkWidget *pBoxRowAddComments = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *pLabelAddComments = gtk_label_new(g_strconcat(_("Add comments: "), ADD_COMMENTS_DEFAULT, NULL));
    GtkWidget *pSwitchAddComments = gtk_switch_new();

    gtk_box_append(GTK_BOX(pBoxRowFromCode), pLabelFromCode);
    gtk_box_append(GTK_BOX(pBoxRowFromCode), pSwitchFromCode);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowFromCode), GTK_ALIGN_FILL);

    gtk_box_set_homogeneous(GTK_BOX(pBoxRowForcePo), TRUE);

    gtk_box_append(GTK_BOX(pBoxRowForcePo), pBoxLabelForcePo);
    gtk_box_append(GTK_BOX(pBoxRowForcePo), pBoxSwitchForcePo);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowForcePo), GTK_ALIGN_FILL);

    gtk_box_append(GTK_BOX(pBoxRowPackageVersion), pLabelPackageVersion);
    gtk_box_append(GTK_BOX(pBoxRowPackageVersion), pSwitchPackageVersion);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowPackageVersion), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowOutput), pLabelOutput);
    gtk_box_append(GTK_BOX(pBoxRowOutput), pSwitchOutput);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowOutput), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowCopyrightHolder), pLabelCopyrightHolder);
    gtk_box_append(GTK_BOX(pBoxRowCopyrightHolder), pSwitchCopyrightHolder);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowCopyrightHolder), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowCplusplus), pLabelCplusplus);
    gtk_box_append(GTK_BOX(pBoxRowCplusplus), pSwitchCplusplus);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowCplusplus), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowOutputDir), pLabelOutputDir);
    gtk_box_append(GTK_BOX(pBoxRowOutputDir), pSwitchOutputDir);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowOutputDir), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowAddComments), pLabelAddComments);
    gtk_box_append(GTK_BOX(pBoxRowAddComments), pSwitchAddComments);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowAddComments), GTK_ALIGN_END);

    gtk_box_append(GTK_BOX(pBoxRowPackageName), pLabelPackageName);
    gtk_box_append(GTK_BOX(pBoxRowPackageName), pSwitchPackageName);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRowPackageName), GTK_ALIGN_END);

    /*  pEntryPackageName = gtk_entry_new();
     gtk_entry_set_placeholder_text(GTK_ENTRY(pEntryPackageName), PACKAGE_NAME_PLACEHOLDER);
     gtk_entry_set_icon_from_icon_name(GTK_ENTRY(pEntryPackageName), GTK_ENTRY_ICON_PRIMARY, "help-about");
     gtk_entry_set_icon_tooltip_text(GTK_ENTRY(pEntryPackageName), GTK_ENTRY_ICON_PRIMARY, _("--package-name=PACKAGE set package name in output"));
  */
    pEntryMsgIdBugsAddress = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pEntryMsgIdBugsAddress), MSG_ID_BUGS_ADDRESS_PLACEHOLDER);
    gtk_entry_set_icon_from_icon_name(GTK_ENTRY(pEntryMsgIdBugsAddress), GTK_ENTRY_ICON_PRIMARY, "help-about");
    gtk_entry_set_icon_tooltip_text(GTK_ENTRY(pEntryMsgIdBugsAddress), GTK_ENTRY_ICON_PRIMARY, _("--msgid-bugs-address=EMAIL@ADDRESS set report address for msgid bugs"));
    gtk_grid_attach(GTK_GRID(pGridMain), GTK_WIDGET(pEntryMsgIdBugsAddress), 1, 1, 1, 1);

    gtk_box_append(GTK_BOX(pBox), pBoxRowFromCode);
    gtk_box_append(GTK_BOX(pBox), pBoxRowForcePo);
    gtk_box_append(GTK_BOX(pBox), pBoxRowOutputDir);
    gtk_box_append(GTK_BOX(pBox), pBoxRowOutput);
    gtk_box_append(GTK_BOX(pBox), pBoxRowPackageName);
    gtk_box_append(GTK_BOX(pBox), pBoxRowPackageVersion);
    gtk_box_append(GTK_BOX(pBox), pBoxRowCplusplus);
    gtk_box_append(GTK_BOX(pBox), pBoxRowAddComments);
    gtk_box_append(GTK_BOX(pBox), pBoxRowCopyrightHolder);
    // gtk_box_append(GTK_BOX(pBox), pEntryMsgIdBugsAddress);

    g_signal_connect(G_OBJECT(pSwitchFromCode), "state-set", G_CALLBACK(OnSwitchFromCode), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchForcePo), "state-set", G_CALLBACK(OnSwitchForcePo), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchPackageVersion), "state-set", G_CALLBACK(OnSwitchPackageVersion), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchCopyrightHolder), "state-set", G_CALLBACK(OnSwitchCopyrightHolder), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchCplusplus), "state-set", G_CALLBACK(OnSwitchCplusplus), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchOutputDir), "state-set", G_CALLBACK(OnSwitchOutputDir), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchAddComments), "state-set", G_CALLBACK(OnSwitchAddComments), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchPackageName), "state-set", G_CALLBACK(OnSwitchPackageGuess), pXgettext);
    g_signal_connect(G_OBJECT(pSwitchOutput), "state-set", G_CALLBACK(OnSwitchOutput), pXgettext);

    gtk_grid_attach(GTK_GRID(pGridMain), pBox, 1, 0, 1, 1);

    pEntryKeyword = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(pEntryKeyword), _("Add Keyword"));
    gtk_entry_set_icon_from_icon_name(GTK_ENTRY(pEntryKeyword), GTK_ENTRY_ICON_PRIMARY, "help-about");
    gtk_entry_set_icon_tooltip_text(GTK_ENTRY(pEntryKeyword), GTK_ENTRY_ICON_PRIMARY, _("-k --keyword=KEYWORD\nadditional keyword to be looked for\n(without WORD means not to use\ndefault keywords)"));
    gtk_grid_attach(GTK_GRID(pGridMain), GTK_WIDGET(pEntryKeyword), 0, 1, 1, 1);
    // gtk_box_append(GTK_BOX(pBox), pEntryKeyword);

    GtkWidget *pButtonXgettext = gtk_button_new_with_label(_(/*TRANSLATORS:Think create a file .pot*/ "Create pot"));

    g_signal_connect(G_OBJECT(pButtonXgettext), "clicked", G_CALLBACK(OnClickXgettext), pXgettext);
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonXgettext, 0, 2, 1, 1);

    GtkWidget *pButtonCreatePo = gtk_button_new_with_label(_(/*NOTHING:pas DeBlaBla*/ "Create po"));
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonCreatePo, 1, 2, 1, 1);

    //  GtkWidget *pButtonMergePo = gtk_button_new_with_label(_(/*no BlaBlaForMe*/ "Merge po"));
    // gtk_grid_attach(GTK_GRID(pGridMain), pButtonMergePo, 1, 2, 1, 1);

    GtkWidget *pButtonMakePo = gtk_button_new_with_label(_("Make mo"));
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonMakePo, 3, 2, 1, 1);

    GtkWidget *pButtonQuit = gtk_button_new_with_label(_("Quit"));
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonQuit, 4, 2, 1, 1);
    g_signal_connect(G_OBJECT(pButtonQuit), "clicked", G_CALLBACK(OnDestroy), NULL);

    pBufferTextViewLineCommand = gtk_text_buffer_new(NULL);
    pTextViewLineCommand = gtk_text_view_new_with_buffer(GTK_TEXT_BUFFER(pBufferTextViewLineCommand));
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pTextViewLineCommand), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(pTextViewLineCommand), TRUE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(pTextViewLineCommand), TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(pTextViewLineCommand), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(pTextViewLineCommand), TRUE);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(pTextViewLineCommand), 10);
    gtk_text_view_set_right_margin(GTK_TEXT_VIEW(pTextViewLineCommand), 10);
    gtk_text_view_set_top_margin(GTK_TEXT_VIEW(pTextViewLineCommand), 10);
    gtk_text_view_set_bottom_margin(GTK_TEXT_VIEW(pTextViewLineCommand), 10);
    gtk_text_view_set_justification(GTK_TEXT_VIEW(pTextViewLineCommand), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(pGridMain), pTextViewLineCommand, 0, 3, 5, 1);

    __poflashLoadCss();
    gtk_window_present(GTK_WINDOW(pWindowMain));
}

/**
 * @brief charge le theme CSS
 *
 */
static void __poflashLoadCss()
{
    pCssProvider = gtk_css_provider_new();
    const gchar *themePathFile = g_strdup_printf("%s%s", POFLASH_THEME_PATH, POFLASH_THEME_FILE);
    gtk_css_provider_load_from_path(pCssProvider, themePathFile);
    gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(pCssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

/**
 * @brief initialise l'application et le lancement
 *
 * @return int
 */
int poFlashNew()
{
    int status;
    app = gtk_application_new("lekiwidebx.dev.poflash", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    setlocale(LC_ALL, "");
    textdomain(GETTEXT_DOMAIN_NAME);
    bind_textdomain_codeset(GETTEXT_DOMAIN_NAME, "UTF-8");
    bindtextdomain(GETTEXT_DOMAIN_NAME, GETTEXT_DIR_NAME);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    return status;
}

/**
 * @brief reponse de la boite de dialogue de fermeture
 *
 * @param pWidget
 * @param result
 * @param data
 */
void OnResponse(GtkWidget *pWidget, gint result, gpointer data)
{
    switch (result)
    {
    case GTK_RESPONSE_OK:
        g_application_quit(G_APPLICATION(app));
        break;
    case GTK_RESPONSE_CANCEL:
    default:
        gtk_window_destroy(GTK_WINDOW(pWidget));
        gtk_widget_show(pWindowMain);
    }
}

/**
 * @brief boite de dialogue qui permet de fermer l'application ou non
 *
 * @param pWidget
 * @param pData
 */
void OnDestroy(GtkWidget *pWidget, gpointer pData)
{
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    pDialogBoxQuit = gtk_dialog_new_with_buttons(_("Confirm QUIT"),
                                                 GTK_WINDOW(pWindowMain),
                                                 flags,
                                                 _("_Cancel"),
                                                 GTK_RESPONSE_CANCEL,
                                                 _("_Ok"),
                                                 GTK_RESPONSE_OK,
                                                 NULL);
    gtk_widget_show(pDialogBoxQuit);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(pDialogBoxQuit));
    const char *message = _("Do you really want to quit ?");
    GtkWidget *labelBoxQuit = gtk_label_new(message);
    gtk_box_append(GTK_BOX(content_area), labelBoxQuit);
    g_signal_connect(GTK_WIDGET(pDialogBoxQuit), "response", G_CALLBACK(OnResponse), NULL);
    gtk_widget_show(pDialogBoxQuit);
}

/**
 * @brief cree une liste de fichiers venant du drag and drop
 *
 * @param target self
 * @param value fichier droppé
 * @param x
 * @param y
 * @param data xgettext_args
 * @return gboolean
 */
static gboolean OnDrop(GtkDropTarget *target, const GValue *value, double x, double y, gpointer data)
{
    static GList *listFile = NULL;
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;

    if (G_VALUE_HOLDS(value, G_TYPE_FILE))
    {
        g_printf("DEBUG: File dropped base_name: %s %s\n", G_VALUE_TYPE_NAME(value), G_VALUE_FILE_BASENAME(value));
        g_printf("DEBUG: get parse name parse_name: %s\n", G_VALUE_FILE_PARSE_NAME(value));
        // gchar *valparsename = G_VALUE_FILE_PARSE_NAME(value); //| /path/du/fichier/filename.ext   tout
        gchar *valbasename = G_VALUE_FILE_BASENAME(value); //| filename.ext                    nom du fichier
        gchar *valpath = G_VALUE_FILE_PATH(value);         //| /path/du/repertoire             path seulement
        listFile = g_list_append(listFile, valbasename);
        g_printf("DEBUG: path dropped path: %s\n", valpath);
        GString *buf = g_string_new("");
        for (GList *l = listFile; l; l = l->next)
        {
            g_string_append(buf, g_strconcat(g_shell_quote(g_strdup((const gchar *)l->data)), " ", NULL));
        }
        g_printf("DEBUG: buf : %s\n", (char *)buf->str);
        data_xgettext_args->inputfile = g_strdup((char *)buf->str);
        g_printf("DEBUG: pXgettext->inputfile:|%s|\n", data_xgettext_args->inputfile);
        g_string_free(buf, TRUE);
    }
    else if (G_VALUE_HOLDS(value, GDK_TYPE_PIXBUF))
        // g_printf("Pixbuf dropped: %s\n", g_value_get_string(value));
        ;
    else
        return FALSE;
    return TRUE;
}

/**
 * construit la ligne de commande xgettext en fonction des parametres de data xgettext_args
 * @param data pXgettext
 */
static void OnClickXgettext(GtkWidget *pWidget, gpointer data)
{
    /*  xgettext -f FILE -D "/home/jean/Senku2-64/" --default-domain=SENKU -o outputTest.pot
     -p "/home/jean/testXgettext/" -L "C" -kN_ -k_ --force-po --from-code=UTF-8 --copyright-holder=LeKiwiDeBx
     --package-name=Senku --package-version=1.0 --msgid-bugs-address=LeKiwiDeBx.team.translate@free.fr */

    //  xgettext -k_ -j -lC -c -s -o po/hello.pot hello.c
    //  xgettext --language=C --keyword=_ --keyword=N_ --from-code=UTF-8 -o messages.pot $(find . -name "*.c" -o -name "*.h")

    /*
    xgettext
        main.c
        -kN_
        -k_
        -p locale
        -o template2.pot
        --add-comments=TRANSLATORS:
        --from-code=UTF-8
        --copyright-holder="Le Kiwi De Bx"
        --package-name="PoFlash"
        --package-version="1.0"
        --msgid-bugs-address="LeKiwiDeBx.translate@lekiwidebx.org"
    */

    /*
    https://docs.python.org/fr/3/library/gettext.html
utilities python gettext
    gettext.bindtextdomain(domain, localedir=None)
 Lie domain au répertoire localedir des localisations.
 Plus spécifiquement, gettext va chercher les fichiers binaires .mo pour un domaine donné,
 en utilisant le chemin suivant (sous Unix) : localedir/language/LC_MESSAGES/domain.mo,
 où language est recherché dans l'une des variables d'environnement suivantes :
 LANGUAGE, LC_ALL, LC_MESSAGES et LANG.
 Si localedir n'est pas renseigné ou vaut None, alors le lien actuel de domain est renvoyé. [1]
    */
    xgettext_args pXgettext = (xgettext_args)malloc(sizeof(struct s_xgettext_args));
    if (!pXgettext)
    {
        g_printf("ERROR: malloc failed\n");
        return;
    }

    pXgettext = (struct s_xgettext_args *)data;
    pXgettext->keyword = g_strdup(GTK_GET_TEXT_ENTRY(pEntryKeyword));
    /* gchar *packageName = g_strdup(g_strstrip((gchar *)GTK_GET_TEXT_ENTRY(pEntryPackageName)));
    if (g_strcmp0(packageName, ""))
        pXgettext->package_name = g_strdup(g_strconcat("--package-name=", packageName, NULL));
    else
    {
        pXgettext->package_name = "";

    } */
    // g_printf("DEBUG: getPackageGuessIntoDir:|%s|\n", __getPackageGuessIntoDir("."));
    // pXgettext->package_name = g_strdup(__getPackageGuessIntoDir("."));

    const gchar *pattern = "(\\w+)";
    GRegex *regex = g_regex_new(pattern, 0, 0, NULL);
    pXgettext->keyword = g_regex_replace(regex, pXgettext->keyword, -1, 0, "-k\\0", 0, NULL);

    pXgettext->msgid_bugs_address = g_strdup(GTK_GET_TEXT_ENTRY(pEntryMsgIdBugsAddress));
    gchar *msgIdBugsAddress = g_strdup(g_strstrip((gchar *)GTK_GET_TEXT_ENTRY(pEntryMsgIdBugsAddress)));
    if (msgIdBugsAddress != NULL)
    {
        if (g_strcmp0(msgIdBugsAddress, ""))
            pXgettext->msgid_bugs_address = g_strdup(g_strconcat("--msgid-bugs-address=", msgIdBugsAddress, NULL));
        else
            pXgettext->msgid_bugs_address = "";
    }
    g_free(msgIdBugsAddress);

    g_printf("DEBUG: pXgettext->inputfile:|%s|\n", pXgettext->inputfile);
    g_printf("DEBUG: pXgettext->keyword:|%s|\n", pXgettext->keyword);
    g_printf("DEBUG: pXgettext->from_code:|%s|\n", pXgettext->from_code);
    g_printf("DEBUG: pXgettext->force_po:|%s|\n", pXgettext->force_po);
    g_printf("DEBUG: pXgettext->copyright_holder:|%s|\n", pXgettext->copyright_holder);
    g_printf("DEBUG: pXgettext->package_version:|%s|\n", pXgettext->package_version);
    g_printf("DEBUG: pXgettext->package_name:|%s|\n", pXgettext->package_name);
    g_printf("DEBUG: pXgettext->output:|%s|\n", pXgettext->output);
    g_printf("DEBUG: pXgettext->msgid_bugs_address:|%s|\n", pXgettext->msgid_bugs_address);
    g_printf("DEBUG: pXgettext->cplusplus:|%s|\n", pXgettext->cplusplus);
    g_printf("DEBUG: pXgettext->output_dir:|%s|\n", pXgettext->output_dir);
    g_printf("DEBUG: pXgettext->add_comments:|%s|\n", pXgettext->add_comments);
    
    gchar *cmdline = g_strdup_printf("xgettext %s %s %s %s %s %s %s %s %s %s %s %s",
                                     pXgettext->output_dir,
                                     pXgettext->output,
                                     pXgettext->cplusplus,
                                     pXgettext->from_code,
                                     pXgettext->add_comments,
                                     pXgettext->keyword,
                                     pXgettext->force_po,
                                     pXgettext->copyright_holder,
                                     pXgettext->package_name,
                                     pXgettext->package_version,
                                     pXgettext->msgid_bugs_address,
                                     pXgettext->inputfile);
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(pBufferTextViewLineCommand), cmdline, -1);
    /* pBufferTextViewLineCommand
    pTextViewLineCommand */

    // g_spawn_command_line_async("xgettext -L C ./*", NULL);
}

/**
 * @brief devine le nom du package dans le repertoire en se basant sur le nom d'un fichier compilé type data)
 *
 * @param pDir repertoire contenant les fichiers
 * @return gchar* le nom du package
 */
static gchar *__getPackageGuessIntoDir(const gchar *pDir)
{
    const gchar *f;
    GDir *dir = g_dir_open(pDir, 0, NULL);
    if (dir)
    {
        while ((f = g_dir_read_name(dir)))
        {
            if (!g_file_test(f, G_FILE_TEST_IS_DIR))
                if (g_file_test(f, G_FILE_TEST_IS_EXECUTABLE))
                {
                    g_printf("DEBUG: fileName: %s\n", f);
                    const gchar *cmdline = g_strdup_printf("file -e soft %s", f);
                    // https://cpp.hotexamples.com/examples/-/-/g_spawn_sync/cpp-g_spawn_sync-function-examples.html
                    gchar *output = NULL;
                    GError *error = NULL;
                    g_spawn_command_line_sync(cmdline, &output, NULL, 0, NULL);
                    g_printf("DEBUG: AVANT REGEX => : |%s|\n", output);
                    GMatchInfo *match_info;
                    const gchar *pattern = ":\\s{1}(\\w+)";
                    const gint match_num = 1;
                    GRegex *regex = g_regex_new(pattern, 0, 0, &error);
                    /* if(!regex)
                     g_critical("DEBUG Error not create regex %s with pattern: ", strerror(errno),pattern); */
                    g_regex_match(regex, output, 0, &match_info);
                    while (g_match_info_matches(match_info))
                    {
                        gchar *word = g_match_info_fetch(match_info, match_num);
                        g_print("Found: |%s|\n", word);
                        if (!g_strcmp0(word, "data"))
                            return g_strstrip(g_strdup(g_strsplit(output, ":", 0)[0]));
                        g_free(word);
                        g_match_info_next(match_info, NULL);
                    }
                    g_match_info_free(match_info);
                    g_regex_unref(regex);
                    g_free(output);
                }
        }
        g_dir_close(dir);
    }
    return NULL;
}

/**
 * @brief write from-code option --from-code
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchFromCode(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->from_code = g_strconcat("--from-code=", UTF8, NULL);
    }
    else
    {
        data_xgettext_args->from_code = "";
    }
    return TRUE;
}

/**
 * @brief write force-po option --force-po
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchForcePo(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->force_po = "--force-po";
    }
    else
    {
        data_xgettext_args->force_po = "";
    }
    return TRUE;
}

/**
 * @brief write package version option --package-version
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchPackageVersion(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->package_version = g_strconcat("--package-version=", PACKAGE_VERSION_DEFAULT, NULL);
    }
    else
    {
        data_xgettext_args->package_version = "";
    }
    return TRUE;
}

/**
 * @brief write copyright holder option --copyright-holder
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchCopyrightHolder(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->copyright_holder = g_strconcat("--copyright-holder=", COPYRIGHT_HOLDER_DEFAULT, NULL);
    }
    else
    {
        data_xgettext_args->copyright_holder = "";
    }
    return TRUE;
}

/**
 * @brief write cplusplus option --c++
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchCplusplus(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->cplusplus = "--c++";
    }
    else
    {
        data_xgettext_args->cplusplus = "";
    }
    return TRUE;
}

/**
 * @brief write output dir option -p
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchOutputDir(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        if (!g_mkdir_with_parents(OUTPUT_DIR_DEFAULT, 0755))
        {
            data_xgettext_args->output_dir = g_strconcat("-p ", OUTPUT_DIR_DEFAULT, NULL);
        }
        else
            g_critical("Could not create output directory \"%s\": %s",
                       OUTPUT_DIR_DEFAULT, strerror(errno));
    }
    else
    {
        data_xgettext_args->output_dir = "";
    }
    return TRUE;
}

/**
 * @brief write add comments
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchAddComments(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->add_comments = g_strconcat("--add-comments=", ADD_COMMENTS_DEFAULT, NULL);
    }
    else
    {
        data_xgettext_args->add_comments = "";
    }
    return TRUE;
}

/**
 * @brief write guess package name
 *
 * @param pWidget
 * @param state
 * @param data
 * @return gboolean
 */
static gboolean OnSwitchPackageGuess(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;

    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->package_name = g_strconcat("--package-name=", PACKAGE_NAME_DEFAULT, NULL);
    }
    else
    {
        data_xgettext_args->package_name = "";
    }
    return TRUE;
}

static gboolean OnSwitchOutput(GtkWidget *pWidget, gboolean state, gpointer data)
{
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;
    gtk_switch_set_state(GTK_SWITCH(pWidget), state);
    if (state)
    {
        data_xgettext_args->output = g_strconcat("-o ", OUTPUT_DEFAULT, NULL);
    }
    else
    {
        data_xgettext_args->output = "";
    }
    return TRUE;
}

/**
 * @brief bootstrap
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    if (poFlashNew())
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
    return 0;
}