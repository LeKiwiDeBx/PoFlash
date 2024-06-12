
/**
 *
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
#define IMAGE_CURRENT "/home/jean/PoFlash/image/delete2.png"

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
// MODEL VIEW  IMPLEMENTATION
/* definition pour l'item du Model View */

typedef struct _FileListData
{
    GListStore *store;
    GtkImage *delete_icon;
} FileListData;

static GList *listFile = NULL;

/* creation d'un GType */
#define FILE_LIST_ITEM_TYPE (file_list_item_get_type())
G_DECLARE_FINAL_TYPE(FileListItem, file_list_item, FILE, LIST_ITEM, GObject)
typedef struct _FileListItem
{
    GObject parent_instance;
    gchar *filename;
    gchar *file_type;
    GdkPaintable *image;

} FileListItem;                                            // doit etre declare avant G_DEFINE_TYPE
G_DEFINE_TYPE(FileListItem, file_list_item, G_TYPE_OBJECT) // cast avec FILE_LIST_ITEM check avec FILE_LIST_IS_ITEM

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
/* zone scrollable pour listview */
GtkWidget *pSw;
/* pour listview */
GtkWidget *pListView;
/* model pour listview */
GListModel *model;
/*image delete icon */
GtkWidget *delete_icon;
/* bouton delete */
GtkWidget *delete_button;
/* image delete */
GtkWidget *delete_image;

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
 * @brief Appel pour lancer un editeur de text sur le fichier .pot
 * pour creer un fichier po d'une langue choisit
 *
 * @param pWidget
 * @param data
 */
static void OnClickCreatePo(GtkWidget *pWidget, gpointer data);

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

/**
 * @brief ouvre la boite de dialogue pour choisir la langue de l'output
 *
 */
static void OpenDialogBoxLang(GtkWidget *pWidget, gpointer data);

/**
 * @brief manage response of Dialog box lang
 *
 * @param pDialogBoxLang
 * @param response_id
 * @param data
 */
static void OnResponseLang(GtkWidget *pDialogBoxLang, gint response_id, gpointer data);

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
 * @brief construit une ligne avec alignement label et switch
 *
 * @param pBoxRow
 * @param pLabel
 * @param pSwitch
 */
static void __box_row_set_align_label_switch(GtkWidget *pBoxRow, GtkWidget *pLabel, GtkWidget *pSwitch);

/**
 * @brief ajoute un espace dans une chaine si non vide
 *
 * @param paramXgettext
 * @return char*
 */
static char *__addSpace(char *paramXgettext);

/**
 * @brief retourne le mime type d'un fichier
 *
 * @param filename
 * @return const gchar*
 */
static const gchar *__get_file_mime_type(const gchar *filename);

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
 * @brief Create a file list model object
 *
 * @param file_list
 * @param delete_icon
 * @return * GListModel*
 */
static GListModel *create_file_list_model(GList *file_list, GdkPaintable *image);

/**
 * @brief Create a file list view objectmake
 *
 * @param model
 * @param delete_icon
 * @return GtkWidget*
 */
static GtkWidget *create_file_list_view(GListModel *model, GdkPaintable *image);

/**
 * @brief Create a file list data object
 *
 * @param store
 * @param delete_icon
 * @return FileListData*
 */
static FileListData *create_file_list_data(GListStore *store, GdkPaintable *image);

/**
 * @brief Set the up file list item object
 *
 * @param factory
 * @param list_item
 */
static void setup_file_list_item(GtkSignalListItemFactory *factory,
                                 GtkListItem *list_item,
                                 gpointer data);

/**
 * @brief Create a file list factory object
 *
 * @param delete_icon
 * @param store
 * @return GtkListItemFactory*
 */
static GtkListItemFactory *create_file_list_factory(GdkPaintable *image, GListStore *store);

/**
 * @brief Bind a file list item
 *
 * @param factory
 * @param list_item
 */
static void bind_file_list_item(GtkSignalListItemFactory *factory,
                                GtkListItem *list_item,
                                gpointer data);

/**
 * @brief Delete a file list item
 *
 * @param item
 * @param store
 */
static gboolean delete_file(GtkGesture *self, gint n_press, gdouble x, gdouble y, FileListItem *item); // GListStore *store

/**
 * @brief Add a file list item
 *
 * @param store
 * @param filename
 * @param delete_icon
 */
static void add_file(GListStore *store, GFile *filename, GdkPaintable *image);

/**
 * @brief when items changed remove or add
 *
 * @param self
 * @param position
 * @param removed
 * @param added
 * @param user_data
 */
static void items_changed_file(GListModel *self, guint position, guint removed, guint added, gpointer user_data);

xgettext_args pXgettext = NULL;
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
    pXgettext = g_malloc(sizeof(struct s_xgettext_args));

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
    GtkWidget *pLabelForcePo = gtk_label_new(_("Force PO"));
    GtkWidget *pSwitchForcePo = gtk_switch_new();

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

    __box_row_set_align_label_switch(pBoxRowFromCode, pLabelFromCode, pSwitchFromCode);
    __box_row_set_align_label_switch(pBoxRowForcePo, pLabelForcePo, pSwitchForcePo);
    __box_row_set_align_label_switch(pBoxRowPackageVersion, pLabelPackageVersion, pSwitchPackageVersion);
    __box_row_set_align_label_switch(pBoxRowOutput, pLabelOutput, pSwitchOutput);
    __box_row_set_align_label_switch(pBoxRowCopyrightHolder, pLabelCopyrightHolder, pSwitchCopyrightHolder);
    __box_row_set_align_label_switch(pBoxRowCplusplus, pLabelCplusplus, pSwitchCplusplus);
    __box_row_set_align_label_switch(pBoxRowOutputDir, pLabelOutputDir, pSwitchOutputDir);
    __box_row_set_align_label_switch(pBoxRowAddComments, pLabelAddComments, pSwitchAddComments);
    __box_row_set_align_label_switch(pBoxRowPackageName, pLabelPackageName, pSwitchPackageName);

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

    GtkWidget *pButtonXgettext = gtk_button_new_with_label(_(/*TRANSLATORS:Think create a file .pot*/ "Create pot"));

    g_signal_connect(G_OBJECT(pButtonXgettext), "clicked", G_CALLBACK(OnClickXgettext), pXgettext);
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonXgettext, 0, 2, 1, 1);

    GtkWidget *pButtonCreatePo = gtk_button_new_with_label(_(/*NOTHING:pas DeBlaBla*/ "Create po"));
    g_signal_connect(G_OBJECT(pButtonCreatePo), "clicked", G_CALLBACK(OpenDialogBoxLang), NULL);
    gtk_grid_attach(GTK_GRID(pGridMain), pButtonCreatePo, 1, 2, 1, 1);

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

    pSw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(pSw), 200);
    gtk_grid_attach(GTK_GRID(pGridMain), pSw, 0, 4, 5, 4);

    gtk_widget_set_size_request(GTK_WIDGET(delete_image), 25, 25);
    model = create_file_list_model(listFile, NULL);
    pListView = create_file_list_view(model, NULL);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(pSw), pListView);

    __poflashLoadCss();
    gtk_window_present(GTK_WINDOW(pWindowMain));
}

// Create a function to allocate and initialize this structure:
static FileListData *create_file_list_data(GListStore *store, GdkPaintable *image)
{
    FileListData *data = g_new(FileListData, 1);
    data->store = g_object_ref(store);
    data->delete_icon = g_object_ref(image);
    return data;
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
 * @brief aligne un label justifie gau et un switch justifie droite sur une ligne horizontale
 *
 * @param pBoxRow
 * @param pLabel
 * @param pSwitch
 */
static void __box_row_set_align_label_switch(GtkWidget *pBoxRow, GtkWidget *pLabel, GtkWidget *pSwitch)
{
    gtk_box_append(GTK_BOX(pBoxRow), pLabel);
    gtk_widget_set_halign(GTK_WIDGET(pLabel), GTK_ALIGN_START);
    gtk_label_set_xalign(GTK_LABEL(pLabel), 0.0);
    gtk_widget_set_hexpand(GTK_WIDGET(pLabel), TRUE);
    gtk_box_append(GTK_BOX(pBoxRow), pSwitch);
    gtk_widget_set_halign(GTK_WIDGET(pSwitch), GTK_ALIGN_END);
    gtk_widget_set_halign(GTK_WIDGET(pBoxRow), GTK_ALIGN_FILL);
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

//////////////////////////////// MODEL VIEW IMPLEMENTATION //////////////////////////////////////////
static GListModel *create_file_list_model(GList *file_list, GdkPaintable *image)
{
    GListStore *store = g_list_store_new(FILE_LIST_ITEM_TYPE); // create a GListStore G_TYPE_OBJECT FILE_LIST_ITEM_TYPE
    // for (GList *l = file_list; l != NULL; l = l->next)
    // {
    //     const gchar *filename = (const gchar *)l->data; // Glist contient juste le nom du fichier
    //     const gchar *file_type = __get_file_mime_type(filename);
    //     FileListItem *item = g_new(FileListItem, 1); // g_new genere un pointeur sur la structure FileListItem
    //     item->filename = g_strdup(filename);
    //     item->file_type = g_strdup(file_type);
    //     item->delete_icon = g_object_ref(delete_icon);
    //     g_list_store_append(store, item);
    // }
    return G_LIST_MODEL(store);
}

static void
file_list_item_init(FileListItem *self)
{
    self->filename = NULL;
    self->file_type = NULL;
    self->image = NULL;
}

static void
file_list_item_class_init(FileListItemClass *class)
{
}

static FileListItem *list_item_new(GType type, GFile *file, GdkPaintable *image)
{
    FileListItem *item = g_object_new(type, NULL);
    item->filename = g_strdup(g_file_get_basename(file));
    g_printf("filename %s\n", item->filename);
    item->file_type = g_strdup(__get_file_mime_type(g_file_get_parse_name(file)));
    GdkPaintable *image_delete = gtk_image_get_paintable(GTK_IMAGE(gtk_image_new_from_file(IMAGE_CURRENT)));
    item->image = g_object_ref(image_delete);
    return item;
}

static void free_file_list_item(gpointer data)
{
    FileListItem *item = (FileListItem *)data;
    g_free(item->filename);
    g_free(item->file_type);
    g_free(item->image);
    g_object_unref(item);
}

static GtkWidget *create_file_list_view(GListModel *model, GdkPaintable *image)
{
    GtkWidget *list_view = gtk_list_view_new(GTK_SELECTION_MODEL(gtk_single_selection_new(model)),
                                             create_file_list_factory(
                                                 image,
                                                 G_LIST_STORE(model)));
    gtk_list_view_set_show_separators(GTK_LIST_VIEW(list_view), TRUE);
    return list_view;
}

static void setup_file_list_item(GtkSignalListItemFactory *factory,
                                 GtkListItem *list_item,
                                 gpointer data)
{
    FileListItem *item = gtk_list_item_get_item(list_item);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_list_item_set_child(list_item, box);
    GtkWidget *filename_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(box), filename_label);
    GtkWidget *type_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(box), type_label);
    GtkWidget *image = gtk_image_new_from_paintable(NULL);
    gtk_box_append(GTK_BOX(box), image);
}

static gboolean delete_file(GtkGesture *self, gint n_press, gdouble x, gdouble y, FileListItem *item)
{
    guint position = 0;
    if (G_LIST_STORE(model) != NULL && item != NULL)
    {
        gboolean found = g_list_store_find(G_LIST_STORE(model), item, &position);
        if (found)
        {
            g_list_store_remove(G_LIST_STORE(model), position);
            GString *buf = g_string_new("");
            for (GList *l = listFile; l; l = l->next)
            {
                if (!g_strcmp0((const gchar *)l->data, item->filename))
                    listFile = g_list_remove(listFile, l->data);
            }
            // g_list_foreach(listFile, (GFunc)printf, NULL);
            for (GList *l = listFile; l; l = l->next)
                g_string_append(buf, g_strconcat(g_shell_quote(g_strdup((const gchar *)l->data)), " ", NULL));
            pXgettext->inputfile = g_strdup((char *)buf->str);
            g_printf("DEBUG: apres delete pXgettext->inputfile:|%s|\n", pXgettext->inputfile);
        }
    }
    return GDK_EVENT_STOP;
}

static FileListItem *item_copy(FileListItem *item, GListStore *store)
{
    FileListItem *copy = g_memdup2(item, sizeof(FileListItem));
    /*  copy->store = g_object_ref(store);*/
    return copy;
}

static void item_free_full(FileListItem *item)
{
    g_free(item->filename);
    g_free(item->file_type);
    g_free(item->image);
    /*  if (item->store)
     {
         g_object_unref(item->store);
     } */
    g_free(item);
}

static void free_file_list_data(gpointer data)
{
    FileListData *list_data = (FileListData *)data;
    g_object_unref(list_data->store);
    g_free(list_data);
}

static GtkListItemFactory *create_file_list_factory(GdkPaintable *image, GListStore *store)
{
    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_file_list_item), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_file_list_item), NULL);
    return factory;
}

static void bind_file_list_item(GtkSignalListItemFactory *factory,
                                GtkListItem *list_item,
                                gpointer data)
{
    FileListItem *item = gtk_list_item_get_item(list_item);
    GtkWidget *box = gtk_list_item_get_child(list_item);
    GtkWidget *filename_label = gtk_widget_get_first_child(box);
    GtkWidget *type_label = gtk_widget_get_next_sibling(filename_label);
    GtkWidget *image = gtk_widget_get_next_sibling(type_label);
    gtk_label_set_label(GTK_LABEL(filename_label), g_strdup_printf("%-10s", g_strconcat(item->filename, NULL)));
    gtk_label_set_label(GTK_LABEL(type_label), item->file_type);
    gtk_image_set_from_paintable(GTK_IMAGE(image), item->image);
    gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
    GtkGesture *gesture_click = gtk_gesture_click_new();
    gtk_widget_add_controller(GTK_WIDGET(image), GTK_EVENT_CONTROLLER(gesture_click));
    g_signal_connect(gesture_click, "released", G_CALLBACK(delete_file), G_OBJECT(item));
    g_signal_connect(G_LIST_MODEL(model), "items-changed", G_CALLBACK(items_changed_file), G_OBJECT(item));
}
static void add_file(GListStore *store, GFile *filename, GdkPaintable *image)
{
    g_list_store_append(store, list_item_new(FILE_LIST_ITEM_TYPE, filename, image));
    // g_signal_emit_by_name(G_LIST_MODEL(store), "items-changed", 0, 1, 1, 1);
}

static void items_changed_file(GListModel *self, guint position, guint removed, guint added, gpointer user_data)
{
    g_printf("DEBUG: items_changed_file\n");
    FileListItem *item = (FileListItem *)user_data;
    g_printf("DEBUG: CHANGED ITEMS item->filename:|%s|\n", item->filename);
    // GListModel
}

/*
In GTK 4, the
GListModel
 interface is used to provide data to the
GtkListView
. When the data in the model changes, you need to emit specific signals to notify the
GtkListView
 about the changes.

The
GListModel
 interface defines the following signals that you can use to notify the
GtkListView
 about changes:

items-changed:
This signal is emitted when one or more items in the model have been changed, added,
or removed. You should emit this signal when you modify the contents of the model.
g_signal_emit_by_name(G_LIST_MODEL(store), "items-changed", 0, position, 0, n_items);<br>

Here,
position  is the index of the first item that changed, and
n_items is the number of items that changed.
cleared:
This signal is emitted when all items in the model have been removed. You should emit this
signal when you clear the entire model.
g_signal_emit_by_name(G_LIST_MODEL(store), "cleared", 0);<br>


In the case of the
GListStore
 used in the previous example, you can emit the
items-changed signal when you add, remove, or modify items in the store.
Here's an example of how you can emit the
items-changed signal when adding a new item to the GListStore
:*/

/*
In this example, after appending the new item to the GListStore, we emit the
items-changed signal with the position of the added item and a count of 1 (since we added one item).

Similarly, you can emit the items-changed signal when removing or modifying items in the
GListStore
.

By emitting the appropriate signals, the GtkListView
 will be notified of the changes in the model and will update its display accordingly.
*/

///////////////////////////////////////////////////////////
static void OnClickCreatePo(GtkWidget *pWidget, gpointer data)
{ // -p -o --package-name
    /*  pXgettext->output     // -o PoFlash.pot
        pXgettext->output_dir // -p locale
        pXgettext->package_name // --package-name=PoFlash */
    //  choisr la langue lang

    //  creer / ouvrir le repertoire langue locale/<lang as fr_FR>/LC_MESSAGES/
    //  creer / ouvrir le fichier PoFlash.po avec recopie du contenu du pot
    //  lancer soit xdg-open ou une editeur preselectionné
    //  ouvrir le fichier package.po
    g_spawn_command_line_async("xdg-open /home/jean/PoFlash/PoFlash.po", NULL);
}

static void OpenDialogBoxLang(GtkWidget *pWidget, gpointer data)
{
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *pDialogBoxLang = gtk_dialog_new_with_buttons(_("Choose language"),
                                                            GTK_WINDOW(pWindowMain),
                                                            flags,
                                                            _("_Cancel"),
                                                            GTK_RESPONSE_CANCEL,
                                                            _("_Ok"),
                                                            GTK_RESPONSE_OK,
                                                            NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(pDialogBoxLang));
    const char *message = _("Choose lang to translate");
    GtkWidget *labelBoxLang = gtk_label_new(message);
    gtk_box_append(GTK_BOX(content_area), labelBoxLang);
    const char *langList[] = {"fr_FR", "en_US", "de_DE", "it_IT", "es_ES", NULL};
    GtkWidget *pDropDownLang = gtk_drop_down_new_from_strings(langList);
    GtkWidget *pSw = gtk_scrolled_window_new();
    gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(pSw), 50);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(pSw), pDropDownLang);
    gtk_box_append(GTK_BOX(content_area), pSw);
    g_signal_connect(GTK_WIDGET(pDialogBoxLang), "response", G_CALLBACK(OnResponseLang), (gpointer)pDropDownLang);
    gtk_widget_show(pDialogBoxLang);
}

static void OnResponseLang(GtkWidget *pDialogBoxLang, gint response_id, gpointer data)
{
    switch (response_id)
    {
    case GTK_RESPONSE_OK:
        const char *lang = gtk_string_object_get_string(gtk_drop_down_get_selected_item(GTK_DROP_DOWN(data)));
        if (lang)
        {
            g_printf("DEBUG: OnResponseLang lang:|%s|\n", lang);
            gtk_window_destroy(GTK_WINDOW(pDialogBoxLang));
        }
        break;
    case GTK_RESPONSE_CANCEL:
    default:
        gtk_window_destroy(GTK_WINDOW(pDialogBoxLang));
        gtk_widget_show(GTK_WIDGET(pWindowMain));
    }
}

/**
 * @brief Get the mime type of a file
 *
 * @param filename The name of the file
 * @return The mime type of the file, or NULL if an error occurs
 */
static const gchar *__get_file_mime_type(const gchar *filename)
{
    // const gchar *cQuery = G_FILE_ATTRIBUTE_STANDARD_SIZE "," G_FILE_ATTRIBUTE_TIME_MODIFIED "," G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE "," G_FILE_ATTRIBUTE_UNIX_UID "," G_FILE_ATTRIBUTE_UNIX_GID "," G_FILE_ATTRIBUTE_ACCESS_CAN_READ "," G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE "," G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE;
    const gchar *cQuery = G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE;
    GError *err = NULL;
    // gchar *testFile = "/home/jean/PoFlash/test space.txt";
    GFile *file = g_file_new_for_path(filename);
    GFileInfo *info = g_file_query_info(file,
                                        cQuery,
                                        G_FILE_QUERY_INFO_NONE,
                                        NULL, &err);
    g_object_unref(file);
    if (err)
    {
        g_printerr("ERROR: %s\n", err->message);
        g_error_free(err);
        return NULL;
    }

    if (info)
    {
        const gchar *mime_type = g_strdup(g_file_info_get_content_type(info)); // g_content_type_get_description g_content_type_get_mime_type
        g_printf("DEBUG: get mime type mime_type: %s\n", g_strdup(g_file_info_get_content_type(info)));
        g_object_unref(info);
        return mime_type;
    }
    else
    {
        return NULL;
    }
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
    // static GList *listFile = NULL; // liste de fichiers globale car sera dans la View  du GListModel
    xgettext_args data_xgettext_args = (struct s_xgettext_args *)data;

    if (G_VALUE_HOLDS(value, G_TYPE_FILE))
    {
        g_printf("DEBUG: File dropped base_name: %s %s\n", G_VALUE_TYPE_NAME(value), G_VALUE_FILE_BASENAME(value));
        g_printf("DEBUG: get parse name parse_name: %s\n", G_VALUE_FILE_PARSE_NAME(value));
        // MAJ du store
        add_file(G_LIST_STORE(model), G_FILE(g_value_get_object(value)), NULL);

        ;                                                     // TODO: a externaliser pour le add_file and delete_file du data_xgettext_args->inputfile updated //////////////////////////
        gchar *valparsename = G_VALUE_FILE_PARSE_NAME(value); //| /path/du/fichier/filename.ext   tout
        gchar *valbasename = G_VALUE_FILE_BASENAME(value);    //| filename.ext                    nom du fichier
        g_printf("DEBUG LEVEL ON DROP: get mime type mime_type: %s\n", __get_file_mime_type(valparsename));
        gchar *valpath = G_VALUE_FILE_PATH(value); //| /path/du/repertoire             path seulement
        // MAJ du data_xgettext_args->inputfile
        listFile = g_list_append(listFile, valbasename);
        g_printf("DEBUG: path dropped path: %s\n", valpath);
        GString *buf = g_string_new("");
        for (GList *l = listFile; l; l = l->next)
        {
            g_string_append(buf, g_strconcat(g_shell_quote(g_strdup((const gchar *)l->data)), " ", NULL));
        }
        g_printf("DEBUG: buf : %s\n", (char *)buf->str);
        ; // END OF TODO: a externaliser /////////////////////////////////////////////////////////////

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

    gchar *cmdline = g_strdup_printf("xgettext %s%s%s%s%s%s%s%s%s%s%s%s",
                                     __addSpace(pXgettext->output_dir),
                                     __addSpace(pXgettext->output),
                                     __addSpace(pXgettext->cplusplus),
                                     __addSpace(pXgettext->from_code),
                                     __addSpace(pXgettext->add_comments),
                                     __addSpace(pXgettext->keyword),
                                     __addSpace(pXgettext->force_po),
                                     __addSpace(pXgettext->copyright_holder),
                                     __addSpace(pXgettext->package_name),
                                     __addSpace(pXgettext->package_version),
                                     __addSpace(pXgettext->msgid_bugs_address),
                                     __addSpace(pXgettext->inputfile));
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
                    // g_printf("DEBUG: fileName: %s\n", f);
                    const gchar *cmdline = g_strdup_printf("file -e soft %s", f);
                    // https://cpp.hotexamples.com/examples/-/-/g_spawn_sync/cpp-g_spawn_sync-function-examples.html
                    gchar *output = NULL;
                    GError *error = NULL;
                    g_spawn_command_line_sync(cmdline, &output, NULL, 0, NULL);
                    // g_printf("DEBUG: AVANT REGEX => : |%s|\n", output);
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
                        // g_print("Found: |%s|\n", word);
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
 * @brief ajoute un espace entre les parametres
 *
 * @param paramXgettext
 * @return char*
 */
static char *__addSpace(char *paramXgettext)
{
    if (paramXgettext == NULL || strlen(paramXgettext) == 0)
    {
        return ""; // return empty string if paramXgettext is empty
    }
    else
    {
        char *result = malloc(strlen(paramXgettext) + 2); // +1 for the space, +1 for the null terminator
        sprintf(result, "%s ", paramXgettext);
        return result;
    }
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