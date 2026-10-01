include $(TOPDIR)/rules.mk

PKG_NAME:=libserialport
PKG_VERSION:=0.1.2
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/sigrokproject/libserialport.git
PKG_SOURCE_VERSION:=21b3dfe5f68c205be4086469335fd2fc2ce11ed2
PKG_MIRROR_HASH:=skip

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libserialport
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=Serial port library
	URL:=https://sigrok.org/wiki/Libserialport
endef

define Package/libserialport/description
	Cross-platform library for accessing serial ports
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libserialport.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libserialport.pc $(1)/usr/lib/pkgconfig/
endef

define Package/libserialport/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libserialport))
