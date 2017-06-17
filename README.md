git-credential-gnomekeyring
===========================

git-credential-gnomekeyring is a git credential helper for Gnome Keyring.

WARNING
-------

GNOME keyring support is now included in git, so this project is deprecated.

For example, the official GNOME keyring support is available in the
following Linux distributions.

### Debian/Ubuntu/Mint Linux

There is a git package that includes all of contrib sources in
/usr/share/doc/git/contrib/.
Users who want to use git-credential-gnomekeyring should copy source from
/usr/share/doc/git/contrib/credential/gnome-keyring to their working
directory, then users can make and install it.

### Arch Linux

There is a package.
https://aur.archlinux.org/packages/git-credential-gnome-keyring/

### Fedora 25

Its git-core package includes git-credential-gnome-keyring binary then users
can just use it.

### Fedora 26

In recent git-core `git-credential-gnome-keyring` has been superseded by
`git-credential-libsecret`.
To use it execute `git config --global credential.helper libsecret` in terminal.

Install
-------

	$ git clone https://github.com/shugo/git-credential-gnomekeyring.git
	$ cd git-credential-gnomekeyring/
	$ make
	$ cp git-credential-gnomekeyring /path/to/bin/

Configuration
-------------

Add the following lines to ~/.gitconfig:

	[credential]
		helper = gnomekeyring

Usage
-----

	$ cd /path/to/your/repository/
	$ git remote set-url origin https://<usename>@<host>/<path>
	$ git push origin master
	Password for 'https://shugo@github.com': 
	...
	$ git push origin master
	(Password never asked)
	...
	$

Note that you have to include the username in the URL.

License
-------

	(The MIT License)

	Copyright (c) 2012 Shugo Maeda

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	'Software'), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
