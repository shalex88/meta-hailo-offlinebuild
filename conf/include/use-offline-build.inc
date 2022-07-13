FILESEXTRAPATHS_prepend := "${THISDIR}:"
SRC_URI += "file://support-offline-compilation.patch"

S = "${WORKDIR}/git2"
EXTRA_OECMAKE_append = " -DHAILO_OFFLINE_COMPILATION=ON"

CURRENT_DIR_FULLPATH := "${@os.path.realpath(d.getVar('THISDIR'))}"
DOWNLOAD_DEPENDENCIES_SCRIPT = "${CURRENT_DIR_FULLPATH}/download_external_dependencies.sh"
TAR_FILE_PATH = "${DL_DIR}/hailort-${PN}.tar.gz"

python do_download_dependecies() {
    git_temp_directory = d.getVar('WORKDIR') + '/temp'
    src_uri = (d.getVar('SRC_URI') or "").split()

    try:
        fetcher = bb.fetch2.Fetch(src_uri, d)
        fetcher.unpack(git_temp_directory)
    except bb.fetch2.BBFetchException as e:
        bb.fatal("Bitbake Fetcher Error: " + repr(e))

    # Download dependecies
    git_directory = git_temp_directory + '/git'
    bb.process.run(d.getVar('DOWNLOAD_DEPENDENCIES_SCRIPT'), cwd=git_directory)

    # Unpack to tar.gz
    bb.process.run('tar -czvf {} .'.format(d.getVar('TAR_FILE_PATH')), cwd=git_directory)
}

# TODO: run only once with internet enabled, comment afterwards
do_fetch[postfuncs] += "do_download_dependecies"

do_unpack() {
    tar -xvf ${TAR_FILE_PATH} -C ${S}
}