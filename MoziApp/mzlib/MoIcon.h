#pragma once

// ��Ҫ����ͼ��궨����滻����ͼ����������޸�ʱֻҪ�����޸ľ���

// �ƻ���Ŀ����ʱʹ�õ�ͼ��
#define PLAN_DEFAULT_ICON			ICON_FA_PLACE_OF_WORSHIP		// ��mzui.cpp�е��½��ƻ��У�AddNewPlanPopup��ʹ��
#define FILETREE_ICON_PLAN			ICON_FA_PLACE_OF_WORSHIP		// ����Դ��������ʹ��ͼ��


#define TARGET_DEFAULT_ICON			ICON_FA_ANCHOR


// �ļ���ͼ��
// �����ķ���Ķ���һ��
#define FILETREE_ICON_NOFILE		ICON_FA_FILE

// �Ѿ��ֺ�����ļ���ͼ��
#define FILETREE_ICON_FOLDER		ICON_FA_FOLDER
#define FILETREE_ICON_TEXTFILE		ICON_FA_FILE_LINES
#define FILETREE_ICON_WORD			ICON_FA_FILE_WORD
#define FILETREE_ICON_PDF			ICON_FA_FILE_PDF
#define FILETREE_ICON_PNG			ICON_FA_FILE_IMAGE
#define FILETREE_ICON_JPG			ICON_FA_FILE_IMAGE
#define FILETREE_ICON_XLSX			ICON_FA_FILE_EXCEL
#define FILETREE_ICON_PPT			ICON_FA_FILE_POWERPOINT
#define FILETREE_ICON_VOICE			ICON_FA_FILE_AUDIO
#define FILETREE_ICON_VIDEO			ICON_FA_FILE_VIDEO
#define FILETREE_ICON_ZIPPER		ICON_FA_FILE_ZIPPER
#define FILETREE_ICON_CODE			ICON_FA_FILE_CODE


#define FILETREE_ICON_BIN			Moui::IconAndChinese(ICON_FA_TRASH, u8"����վ",1).c_str()		// ����վר��ͼ��




// ��ҳ��ݹ�����ͼ��
#define HOMEPAGE_IOCN_FA_ARROW_LEFT				ICON_FA_ARROW_LEFT
#define HOMEPAGE_IOCN_FA_ARROW_RIGHT			ICON_FA_ARROW_RIGHT
#define HOMEPAGE_IOCN_FA_FOLDER_PLUS			ICON_FA_FOLDER_PLUS
#define HOMEPAGE_IOCN_FA_FOLDER_OPEN			ICON_FA_FOLDER_OPEN
#define HOMEPAGE_IOCN_FA_FLOPPY_DISK			ICON_FA_FLOPPY_DISK
#define HOMEPAGE_IOCN_FA_PLAY					ICON_FA_PLAY
#define HOMEPAGE_IOCN_FA_DOWNLOAD				ICON_FA_DOWNLOAD
#define HOMEPAGE_IOCN_FA_UPLOAD					ICON_FA_UPLOAD

// ����ʼҳ����������
// �ļ���
#define HONEPAGENAME_FILE			    Moui::IconAndChinese(ICON_FA_FOLDER, u8"�ļ�", 1).c_str()
#define HONEPAGENAME_SUBFILE_NEW		Moui::IconAndChinese(ICON_FA_FILE, u8"�½�").c_str()
#define HONEPAGENAME_SUBNEW_NEWFILE		Moui::IconAndChinese(ICON_FA_FILE_LINES, u8"�½��ı�").c_str()
#define HONEPAGENAME_SUBNEW_NEWFOLDER	Moui::IconAndChinese(ICON_FA_FOLDER_PLUS, u8"�½��ļ���").c_str()

#define HONEPAGENAME_SUBFILE_OPEN       Moui::IconAndChinese(ICON_FA_FOLDER_OPEN, u8"��").c_str()
#define HONEPAGENAME_SUBFILE_SAVE       Moui::IconAndChinese(ICON_FA_FLOPPY_DISK, u8"����").c_str()//ICON_FA_FLORIN_SIGN
#define HONEPAGENAME_SUBFILE_CLOSE      Moui::IconAndChinese(ICON_FA_RECTANGLE_XMARK, u8"�ر�").c_str()
#define HONEPAGENAME_SUBFILE_EXIT       Moui::IconAndChinese(ICON_FA_CIRCLE_XMARK, u8"�˳�").c_str()

// ������
#define HONEPAGENAME_SETTINGS 				Moui::IconAndChinese(ICON_FA_GEAR, u8"����", 1).c_str()
#define HONEPAGENAME_SUBSETTINGS_THEME		Moui::IconAndChinese(ICON_FA_PALETTE, u8"����").c_str()
#define HONEPAGENAME_SUBSETTINGS_DARK			Moui::IconAndChinese("", u8"��ɫ").c_str()
#define HONEPAGENAME_SUBSETTINGS_LIGHT			Moui::IconAndChinese("", u8"��ɫ").c_str()
#define HONEPAGENAME_SUBSETTINGS_CLASSIC		Moui::IconAndChinese("", u8"MOɫ").c_str()

#define HONEPAGENAME_SUBSETTINGS_VIEW					Moui::IconAndChinese(ICON_FA_LAYER_GROUP, u8"��ͼ").c_str()
#define HONEPAGENAME_SUBVIEWS_SOURSEPAGE			Moui::IconAndChinese("", u8"�ļ���Դ������").c_str()
#define HONEPAGENAME_SUBVIEWS_PROPERTYPAGE			Moui::IconAndChinese("", u8"�ļ�����ҳ��").c_str()
#define HONEPAGENAME_SUBVIEWS_CONTENTPAGE			Moui::IconAndChinese("", u8"��ʾҳ��").c_str()
#define HONEPAGENAME_SUBVIEWS_SPACE					Moui::IconAndChinese("", u8"�ļ��Ӵ�").c_str()

// ������
#define HONEPAGENAME_HELP			    Moui::IconAndChinese(ICON_FA_CIRCLE_QUESTION, u8"����", 1).c_str()
#define HONEPAGENAME_SUBHELP_HELPFILE	Moui::IconAndChinese(ICON_FA_HANDSHAKE_ANGLE, u8"�����ĵ�").c_str()
#define HONEPAGENAME_SUBHELP_VERSION	Moui::IconAndChinese(ICON_FA_MONUMENT, u8"����MoZI").c_str()

// ��Դҳ��ݹ�����ͼ��
#define SOURSEPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// ȡ���̶�����
#define SOURSEPAGE_ICON_WINDOWS_NO_MOVE			ICON_FA_LOCK				// �̶�����
#define SOURSEPAGE_ICON_REFLASH_FOLDER			ICON_FA_ARROWS_ROTATE		// ˢ���ļ���

#define SOURSEPAGE_ICON_EXPAND_FOLDERS			ICON_FA_FOLDER_TREE			// չ���ļ���
#define SOURSEPAGE_ICON_COLLAPSE_FOLDERS		ICON_FA_BARS_STAGGERED		// �۵��ļ���
#define SOURSEPAGE_ICON_FA_ARROWS_ROTATE		ICON_FA_ARROWS_ROTATE		// �۵��ļ���

// ��Դҳ�һ��ļ��е����Ӵ�
// �ļ��һ�����
#define SOURSEPAGE_FOLDER_POPUP_ADD						Moui::IconAndChinese(ICON_FA_CIRCLE_QUESTION, u8"���").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWFOLDER		Moui::IconAndChinese(ICON_FA_FOLDER_PLUS, u8"�½��ļ���").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDPLAN			Moui::IconAndChinese(ICON_FA_FILE_ARROW_UP, u8"�½��ƻ�").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_TEXTFILE		Moui::IconAndChinese(ICON_FA_FILE_LINES, u8"�½��ı��ļ�").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_WORDFILE		Moui::IconAndChinese(ICON_FA_FILE_WORD, u8"�½�Word�ĵ�").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_PPTFILE		Moui::IconAndChinese(ICON_FA_FILE_POWERPOINT, u8"�½�PPT�ļ�").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDFILE			Moui::IconAndChinese(ICON_FA_FILE_ARROW_UP, u8"�����ļ�").c_str()


#define SOURSEPAGE_FOLDER_POPUP_CUT						Moui::IconAndChinese(ICON_FA_SCISSORS, u8"����").c_str()
#define SOURSEPAGE_FOLDER_POPUP_COPY					Moui::IconAndChinese(ICON_FA_COPY, u8"����").c_str()
#define SOURSEPAGE_FOLDER_POPUP_PASTE					Moui::IconAndChinese(ICON_FA_PASTE, u8"ճ��").c_str()
#define SOURSEPAGE_FOLDER_POPUP_DELETE					Moui::IconAndChinese(ICON_FA_XMARK, u8"ɾ��").c_str()	
#define SOURSEPAGE_FOLDER_POPUP_RENAME					Moui::IconAndChinese(ICON_FA_FILE_SIGNATURE, u8"������").c_str()	
#define SOURSEPAGE_FOLDER_POPUP_PROPERTY				Moui::IconAndChinese(ICON_FA_FILE_SIGNATURE, u8"����").c_str()	

// ����վ�һ�����
#define SOURSEPAGE_RECYCLE_BIN_POPUP_EMPTY				Moui::IconAndChinese(ICON_FA_XMARK, u8"��ջ���վ").c_str()	
#define SOURSEPAGE_RECYCLE_BIN_POPUP_CUT				Moui::IconAndChinese(ICON_FA_SCISSORS, u8"����").c_str()
#define SOURSEPAGE_RECYCLE_BIN_POPUP_RETURN				Moui::IconAndChinese(ICON_FA_SCISSORS, u8"��ԭ").c_str()
#define SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE				Moui::IconAndChinese(ICON_FA_XMARK, u8"ɾ��").c_str()

// ����ҳ��ݹ�����ͼ��
#define PROPERTYPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// ȡ���̶�����
#define PROPERTYPAGE_ICON_WINDOWS_NO_MOVE		ICON_FA_LOCK				// �̶�����
#define PROPERTYPAGE_ICON_WINDOWS_CHANGE		ICON_FA_FOLDER_PLUS			// �޸���������



// ����ҳ��ݹ�����ͼ��
#define CONTENTPAGE_ICON_BACK					ICON_FA_ARROW_LEFT_LONG	
#define CONTENTPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// ȡ���̶�����
#define CONTENTPAGE_ICON_WINDOWS_NO_MOVE		ICON_FA_LOCK				// �̶�����