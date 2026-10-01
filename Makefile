include $(TOPDIR)/rules.mk

PKG_NAME:=espd
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/espd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=ESP control over ubus
	DEPENDS:=+libserialport +libubus +libubox +libblobmsg-json
endef

define Package/espd/description
	ubus module for controlling ESP microcontrollers over USB serial
endef

define Package/espd/install
	$(INSTALL_DIR) $(1)/usr/bin $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/espd $(1)/usr/bin/
	$(INSTALL_BIN) ./files/espd.init $(1)/etc/init.d/espd
endef

$(eval $(call BuildPackage,espd))
