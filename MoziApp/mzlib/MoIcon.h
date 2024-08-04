#pragma once

// 主要用于图标宏定义的替换，当图标进行总体修改时只要在这修改就行

// 计划和目标暂时使用的图标
#define PLAN_DEFAULT_ICON			ICON_FA_PLACE_OF_WORSHIP		// 在mzui.cpp中的新建计划中（AddNewPlanPopup）使用
#define FILETREE_ICON_PLAN			ICON_FA_PLACE_OF_WORSHIP		// 在资源管理器中使用图标


#define TARGET_DEFAULT_ICON			ICON_FA_ANCHOR


// 文件树图标
// 所有文分类的都用一个
#define FILETREE_ICON_NOFILE		ICON_FA_FILE

// 已经分好类的文件树图标
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


#define FILETREE_ICON_BIN			Moui::IconAndChinese(ICON_FA_TRASH, u8"回收站",1).c_str()		// 回收站专用图标




// 主页快捷工具栏图标
#define HOMEPAGE_IOCN_FA_ARROW_LEFT				ICON_FA_ARROW_LEFT
#define HOMEPAGE_IOCN_FA_ARROW_RIGHT			ICON_FA_ARROW_RIGHT
#define HOMEPAGE_IOCN_FA_FOLDER_PLUS			ICON_FA_FOLDER_PLUS
#define HOMEPAGE_IOCN_FA_FOLDER_OPEN			ICON_FA_FOLDER_OPEN
#define HOMEPAGE_IOCN_FA_FLOPPY_DISK			ICON_FA_FLOPPY_DISK
#define HOMEPAGE_IOCN_FA_PLAY					ICON_FA_PLAY
#define HOMEPAGE_IOCN_FA_DOWNLOAD				ICON_FA_DOWNLOAD
#define HOMEPAGE_IOCN_FA_UPLOAD					ICON_FA_UPLOAD

// 其起始页工具栏设置
// 文件栏
#define HONEPAGENAME_FILE			    Moui::IconAndChinese(ICON_FA_FOLDER, u8"文件", 1).c_str()
#define HONEPAGENAME_SUBFILE_NEW		Moui::IconAndChinese(ICON_FA_FILE, u8"新建").c_str()
#define HONEPAGENAME_SUBNEW_NEWFILE		Moui::IconAndChinese(ICON_FA_FILE_LINES, u8"新建文本").c_str()
#define HONEPAGENAME_SUBNEW_NEWFOLDER	Moui::IconAndChinese(ICON_FA_FOLDER_PLUS, u8"新建文件夹").c_str()

#define HONEPAGENAME_SUBFILE_OPEN       Moui::IconAndChinese(ICON_FA_FOLDER_OPEN, u8"打开").c_str()
#define HONEPAGENAME_SUBFILE_SAVE       Moui::IconAndChinese(ICON_FA_FLOPPY_DISK, u8"保存").c_str()//ICON_FA_FLORIN_SIGN
#define HONEPAGENAME_SUBFILE_CLOSE      Moui::IconAndChinese(ICON_FA_RECTANGLE_XMARK, u8"关闭").c_str()
#define HONEPAGENAME_SUBFILE_EXIT       Moui::IconAndChinese(ICON_FA_CIRCLE_XMARK, u8"退出").c_str()

// 设置栏
#define HONEPAGENAME_SETTINGS 				Moui::IconAndChinese(ICON_FA_GEAR, u8"设置", 1).c_str()
#define HONEPAGENAME_SUBSETTINGS_THEME		Moui::IconAndChinese(ICON_FA_PALETTE, u8"主题").c_str()
#define HONEPAGENAME_SUBSETTINGS_DARK			Moui::IconAndChinese("", u8"黑色").c_str()
#define HONEPAGENAME_SUBSETTINGS_LIGHT			Moui::IconAndChinese("", u8"白色").c_str()
#define HONEPAGENAME_SUBSETTINGS_CLASSIC		Moui::IconAndChinese("", u8"MO色").c_str()

#define HONEPAGENAME_SUBSETTINGS_VIEW					Moui::IconAndChinese(ICON_FA_LAYER_GROUP, u8"视图").c_str()
#define HONEPAGENAME_SUBVIEWS_SOURSEPAGE			Moui::IconAndChinese("", u8"文件资源管理器").c_str()
#define HONEPAGENAME_SUBVIEWS_PROPERTYPAGE			Moui::IconAndChinese("", u8"文件属性页面").c_str()
#define HONEPAGENAME_SUBVIEWS_CONTENTPAGE			Moui::IconAndChinese("", u8"显示页面").c_str()
#define HONEPAGENAME_SUBVIEWS_SPACE					Moui::IconAndChinese("", u8"文件视窗").c_str()

// 帮助栏
#define HONEPAGENAME_HELP			    Moui::IconAndChinese(ICON_FA_CIRCLE_QUESTION, u8"帮助", 1).c_str()
#define HONEPAGENAME_SUBHELP_HELPFILE	Moui::IconAndChinese(ICON_FA_HANDSHAKE_ANGLE, u8"帮助文档").c_str()
#define HONEPAGENAME_SUBHELP_VERSION	Moui::IconAndChinese(ICON_FA_MONUMENT, u8"关于MoZI").c_str()

// 资源页快捷工具栏图标
#define SOURSEPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// 取消固定窗口
#define SOURSEPAGE_ICON_WINDOWS_NO_MOVE			ICON_FA_LOCK				// 固定窗口
#define SOURSEPAGE_ICON_REFLASH_FOLDER			ICON_FA_ARROWS_ROTATE		// 刷新文件夹

#define SOURSEPAGE_ICON_EXPAND_FOLDERS			ICON_FA_FOLDER_TREE			// 展开文件夹
#define SOURSEPAGE_ICON_COLLAPSE_FOLDERS		ICON_FA_BARS_STAGGERED		// 折叠文件夹
#define SOURSEPAGE_ICON_FA_ARROWS_ROTATE		ICON_FA_ARROWS_ROTATE		// 折叠文件夹

// 资源页右击文件夹弹出视窗
// 文件右击操作
#define SOURSEPAGE_FOLDER_POPUP_ADD						Moui::IconAndChinese(ICON_FA_CIRCLE_QUESTION, u8"添加").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWFOLDER		Moui::IconAndChinese(ICON_FA_FOLDER_PLUS, u8"新建文件夹").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDPLAN			Moui::IconAndChinese(ICON_FA_FILE_ARROW_UP, u8"新建计划").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_TEXTFILE		Moui::IconAndChinese(ICON_FA_FILE_LINES, u8"新建文本文件").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_WORDFILE		Moui::IconAndChinese(ICON_FA_FILE_WORD, u8"新建Word文档").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_PPTFILE		Moui::IconAndChinese(ICON_FA_FILE_POWERPOINT, u8"新建PPT文件").c_str()
#define SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDFILE			Moui::IconAndChinese(ICON_FA_FILE_ARROW_UP, u8"现有文件").c_str()


#define SOURSEPAGE_FOLDER_POPUP_CUT						Moui::IconAndChinese(ICON_FA_SCISSORS, u8"剪切").c_str()
#define SOURSEPAGE_FOLDER_POPUP_COPY					Moui::IconAndChinese(ICON_FA_COPY, u8"复制").c_str()
#define SOURSEPAGE_FOLDER_POPUP_PASTE					Moui::IconAndChinese(ICON_FA_PASTE, u8"粘贴").c_str()
#define SOURSEPAGE_FOLDER_POPUP_DELETE					Moui::IconAndChinese(ICON_FA_XMARK, u8"删除").c_str()	
#define SOURSEPAGE_FOLDER_POPUP_RENAME					Moui::IconAndChinese(ICON_FA_FILE_SIGNATURE, u8"重命名").c_str()	
#define SOURSEPAGE_FOLDER_POPUP_PROPERTY				Moui::IconAndChinese(ICON_FA_FILE_SIGNATURE, u8"属性").c_str()	

// 回收站右击操作
#define SOURSEPAGE_RECYCLE_BIN_POPUP_EMPTY				Moui::IconAndChinese(ICON_FA_XMARK, u8"清空回收站").c_str()	
#define SOURSEPAGE_RECYCLE_BIN_POPUP_CUT				Moui::IconAndChinese(ICON_FA_SCISSORS, u8"剪切").c_str()
#define SOURSEPAGE_RECYCLE_BIN_POPUP_RETURN				Moui::IconAndChinese(ICON_FA_SCISSORS, u8"还原").c_str()
#define SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE				Moui::IconAndChinese(ICON_FA_XMARK, u8"删除").c_str()

// 属性页快捷工具栏图标
#define PROPERTYPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// 取消固定窗口
#define PROPERTYPAGE_ICON_WINDOWS_NO_MOVE		ICON_FA_LOCK				// 固定窗口
#define PROPERTYPAGE_ICON_WINDOWS_CHANGE		ICON_FA_FOLDER_PLUS			// 修改属性内容



// 内容页快捷工具栏图标
#define CONTENTPAGE_ICON_BACK					ICON_FA_ARROW_LEFT_LONG	
#define CONTENTPAGE_IOCN_WINDOWS_MOVE			ICON_FA_UNLOCK				// 取消固定窗口
#define CONTENTPAGE_ICON_WINDOWS_NO_MOVE		ICON_FA_LOCK				// 固定窗口