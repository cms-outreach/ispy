;;; Based on batch-ada-indent.el
;;; Copyright (C) 1999 Samuel Tardieu.
;;; Copyright (C) 2005 John Steele Scott
;;; Copyright (C) 2009 Giulio Eulisse

;;; Typical use: define a script called "ispy-indent" containing:
;;;
;;; #! /bin/sh
;;; emacs -batch -l ispy-cpp-indent.el -f ispy-cpp-indent "$"
;;;
;;; then call "ispy-indent **/*.{cc,h,cpp}" to indent your files.

;;; BUG: If a named file does not exist, this program will create it.

;; Original Author: Samuel Tardieu <sam@debian.org>

;;;###autoload

(custom-set-variables '(load-home-init-file t t))
(custom-set-faces)

(setq load-path (append '("~/.elisp"
                         "~/.elisp/apel")
                       load-path
                       '("/usr/share/emacs/site-lisp"
                         "/usr/share/emacs/site-lisp/add-ons"
                         "/usr/share/emacs/site-lisp/auctex"
                         "/usr/share/emacs/site-lisp/calc"
                         "/usr/share/emacs/site-lisp/elip"
                         "/usr/share/emacs/site-lisp/modes"
                         "/usr/share/emacs/site-lisp/vm"
                         "/usr/share/emacs/site-lisp/w3")))

(load-library "lat-keymaps")
(load-library "lat-editenv")
(load-library "lat-devenv")
(load-library "cms-custom")
;disable backup
(setq backup-inhibited t)
;disable auto save
(setq auto-save-default nil)

(defun ispy-cpp-indent ()
"Load each file named on the command line, and indent them as Pascal sources.
Use this from the command line, with `-batch';
it won't work in an interactive Emacs.
For example, invoke \"emacs -batch -l ispy-cpp-indent.el -f ispy-cpp-indent *.cc\""
(if (not noninteractive)
(error "`ispy-cpp-indent' is to be used only with -batch"))
(autoload 'c++-mode "cc-mode" "C++ Editing Mode" t)
(while command-line-args-left
(let ((source (car command-line-args-left)))
(find-file source)
(c++-mode)
(indent-region (point-min) (point-max) nil)
(untabify (point-min) (point-max))
(write-file source))
(setq command-line-args-left (cdr command-line-args-left)))
(message "Done")
(kill-emacs 0))
