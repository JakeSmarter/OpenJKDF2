.SUFFIXES: .deb .gz .tar .tar.gz

OPENJKDF2_DIST_PATH=dists/OpenJKDF2

all: ${OPENJKDF2_DIST_PATH}/Release.gpg ${OPENJKDF2_DIST_PATH}/main/i18n
	chmod -cR a+rX dists

${OPENJKDF2_DIST_PATH}/Release.gpg: ${OPENJKDF2_DIST_PATH}/Release
	gpg --armor --output "$@" --yes --local-user OpenJKDF2 --detach-sign "$<"

${OPENJKDF2_DIST_PATH}/main/i18n:
	ln -svr "$(@F)" "$@"

${OPENJKDF2_DIST_PATH}/Release: \
    ${OPENJKDF2_DIST_PATH}/main/binary-all/Packages \
    ${OPENJKDF2_DIST_PATH}/main/binary-arm64/Packages \
    ${OPENJKDF2_DIST_PATH}/main/binary-armhf/Packages
	-$(RM) "$@"
	RELEASE_TEMP_FILE="$$(mktemp)" && \
	apt-ftparchive release "$(@D)" > "$$RELEASE_TEMP_FILE" && \
	install -vm 0644 "$$RELEASE_TEMP_FILE" "$@" && \
	$(RM) "$$RELEASE_TEMP_FILE"

${OPENJKDF2_DIST_PATH}/main/binary-all/Packages: \
    openjkdf2-updates/openjkdf2-updates.deb \
    openjkdf2-l10n-de/openjkdf2-l10n-de.deb \
    openjkdf2-l10n-pl/openjkdf2-l10n-pl.deb
	dpkg-name -c -o -s "$(@D)" $? && \
	dpkg-scanpackages -h sha1,sha256 "$(@D)" > "$@"
${OPENJKDF2_DIST_PATH}/main/binary-amd64/Packages: \
    openjkdf2/openjkdf2_amd64.deb
	dpkg-scanpackages -h sha1,sha256 "$(@D)" > "$@"
${OPENJKDF2_DIST_PATH}/main/binary-arm64/Packages: \
    openjkdf2/openjkdf2_arm64.deb
	dpkg-name -c -o -s "$(@D)" $? && \
	dpkg-scanpackages -h sha1,sha256 "$(@D)" > "$@"
${OPENJKDF2_DIST_PATH}/main/binary-armhf/Packages: \
    openjkdf2/openjkdf2_armhf.deb
	dpkg-name -c -o -s "$(@D)" $? && \
	dpkg-scanpackages -h sha1,sha256 "$(@D)" > "$@"
${OPENJKDF2_DIST_PATH}/main/binary-i386/Packages: \
    openjkdf2/openjkdf2_i386.deb
	dpkg-scanpackages -h sha1,sha256 "$(@D)" > "$@"

# We cannot just use `dpkg-dev` because we have to manage ownership, permissions, modification time, acls, attributes etc. (with tar)
openjkdf2-updates/openjkdf2-updates.deb: openjkdf2-updates/DEBIAN/debian-binary\
                                         openjkdf2-updates/control.tar.gz \
                                         openjkdf2-updates/data.tar.gz
	ar rvD "$@" $?
openjkdf2-updates/control.tar.gz: openjkdf2-updates/control.tar
	gzip -v9kn "$<"
openjkdf2-updates/control.tar: openjkdf2-updates/DEBIAN/debian-binary \
                               openjkdf2-updates/DEBIAN/control \
                               openjkdf2-updates/DEBIAN/changelog \
                               openjkdf2-updates/DEBIAN/conffiles \
                               openjkdf2-updates/DEBIAN/preinst \
                               openjkdf2-updates/DEBIAN/postinst \
                               openjkdf2-updates/DEBIAN/postrm
	tar -cvf "$@" \
	    --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=none \
	    -C "$(<D)" debian-binary control changelog conffiles && \
	tar -rvf "$@" \
	    --mode=0755 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    -C "$(<D)" preinst postinst postrm
openjkdf2-updates/data.tar.gz: \
    openjkdf2-updates/etc/apt/trusted.gpg.d/OpenJKDF2.asc \
    openjkdf2-updates/etc/apt/sources.list.d/OpenJKDF2.list \
    openjkdf2-updates/usr/share/metainfo/org.openjkdf2.Updates.metainfo.xml
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    -C "$(@D)" etc usr | \
	gzip -v9kcn > "$@"

# Program packages
# TODO: Add AMD64
openjkdf2/openjkdf2_amd64.deb:
	touch -d '1970-01-01T00:00:00Z' "$@"

openjkdf2/openjkdf2_arm64.deb: openjkdf2/DEBIAN/debian-binary \
                               openjkdf2/control.tar.gz \
                               openjkdf2/data.tar.gz
	ar rvD "$@" $?
openjkdf2/control.tar.gz: openjkdf2/DEBIAN/debian-binary \
                          openjkdf2/DEBIAN/control
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=none \
	    -C "$(<D)" debian-binary control | \
	gzip -v9kcn > "$@"
openjkdf2/data.tar.gz: openjkdf2/data.tar
	gzip -v9kn "$<"
openjkdf2/data.tar: \
    openjkdf2/usr/share/appdata/org.openjkdf2.OpenJKDF2.appdata.xml \
    openjkdf2/usr/share/applications/org.openjdkf2.OpenJKDF2.desktop \
    openjkdf2/usr/share/doc/openjkdf2/README.md \
    openjkdf2/usr/share/icons/hicolor/128x128/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/16x16/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/192x192/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/24x24/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/256x256/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/32x32/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/384x384/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/48x48/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/512x512/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/64x64/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/icons/hicolor/96x96/apps/org.openjkdf2.OpenJKDF2.png \
    openjkdf2/usr/share/metainfo/org.openjkdf2.OpenJKDF2.metainfo.xml
	tar -cvhf "$@" \
	    --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    --exclude=usr/share/appdata \
	    -C "$(@D)" usr && \
	tar -rvf "$@" \
	    --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    -C "$(@D)" usr/share/appdata

# TODO: Add ARM hard float
openjkdf2/openjkdf2_armhf.deb:
	touch -d '1970-01-01T00:00:00Z' "$@"

# TODO: Add i386
openjkdf2/openjkdf2_i386.deb:
	touch -d '1970-01-01T00:00:00Z' "$@"

# L10n packages
openjkdf2-l10n-de/openjkdf2-l10n-de.deb: openjkdf2-l10n-de/DEBIAN/debian-binary\
                                         openjkdf2-l10n-de/control.tar.gz \
                                         openjkdf2-l10n-de/data.tar.gz
	ar rvD "$@" $?
openjkdf2-l10n-de/control.tar.gz: openjkdf2-l10n-de/DEBIAN/debian-binary \
                                  openjkdf2-l10n-de/DEBIAN/control
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=none \
	    -C "$(<D)" debian-binary control | \
	gzip -v9kcn > "$@"
openjkdf2-l10n-de/data.tar.gz: \
    openjkdf2-l10n-de/usr/share/locale/de/LC_MESSAGES/openjkdf2.mo
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    -C "$(@D)" usr | \
	gzip -v9kcn > "$@"

openjkdf2-l10n-pl/openjkdf2-l10n-pl.deb: openjkdf2-l10n-pl/DEBIAN/debian-binary\
                                         openjkdf2-l10n-pl/control.tar.gz \
                                         openjkdf2-l10n-pl/data.tar.gz
	ar rvD "$@" $?
openjkdf2-l10n-pl/control.tar.gz: openjkdf2-l10n-pl/DEBIAN/debian-binary \
                                  openjkdf2-l10n-pl/DEBIAN/control
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=none \
	    -C "$(<D)" debian-binary control | \
	gzip -v9kcn > "$@"
openjkdf2-l10n-pl/data.tar.gz: \
    openjkdf2-l10n-pl/usr/share/locale/pl/LC_MESSAGES/openjkdf2.mo
	tar -cvO --mode=0644 --owner 0 --group 0 --mtime='1970-01-01T00:00:00Z' \
	    --no-acls --no-xattrs --no-selinux --sort=name \
	    -C "$(@D)" usr | \
	gzip -v9kcn > "$@"
