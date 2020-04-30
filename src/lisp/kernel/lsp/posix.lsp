(in-package :ext)

(defun enable-interrupt (signal mode &optional lisp-handler)
  "Enable/disable signal, 
   If mode = :ignore, sets a handler that ignores the signal
   If mode = :default, it sets the handler to the default handler.
   If mode = :lisp, define lisp-handler for signal.
   lisp-handler must only be provided, if mode = :lisp.
   lisp-handler should be a function of one argument (the signal)
   Returns :done if successfull,
           signals an error, if signal is not recognized or cannot be set"
  (flet ((handle-lisp-handler (signal-as-int)
           (core::note-signal-handler signal-as-int lisp-handler)
           2)
         (handle-non-lisp-handler (signal-as-int)
           (core::forget-signal-handler signal-as-int)))
    (let ((int-signal (core::external-to-int-signal signal))
          (result nil))
      (cond (int-signal
             (setq result (core:enable-disable-signals
                           int-signal
                           (ecase mode
                             (:ignore  (handle-non-lisp-handler int-signal) 0)
                             (:default (handle-non-lisp-handler int-signal) 1)
                             (:lisp (handle-lisp-handler int-signal)))))
             (if (zerop result)
                 :done
                 (error "Setting signal ~s in mode ~s errored " signal mode)))
            (t (error "Signal ~s not recognized, Handler cannot be set" signal))))))

(defun default-interrupt (signal)
  "Sets the handler to the default handler for signal"
  (enable-interrupt signal :default))

(defun ignore-interrupt (signal)
  "Sets a handler that ignores the signal"
  (enable-interrupt signal :ignore))

(defun get-signal-handler (signal)
  "Get a lisp handler handler for signal, or signals an error, if signal is not recognized"
  (let ((internal-signal (core::external-to-int-signal signal)))
    (if internal-signal
        (core::get-signal-handler internal-signal)
        (error "Signal ~s not recognized, Handler cannot be read" signal))))

(defun set-signal-handler (signal handler)
  "Set a lisp handler handler for signal"
  (enable-interrupt signal :lisp handler))

(in-package :core)

(defparameter *signal-to-function* nil)
(defparameter *cache-signal-alist* nil)

;;; For Signal see https://pubs.opengroup.org/onlinepubs/009695399/basedefs/signal.h.html
(defun external-to-int-signal (signal)
  (let* ((signal-alist (if *cache-signal-alist* *cache-signal-alist* (setq *cache-signal-alist* (core:signal-code-alist))))
         (found (Assoc signal signal-alist)))
    (if found
        (cdr found)
        nil)))

(defun note-signal-handler (signal function)
  (setf (getf *signal-to-function* signal) function))

(defun forget-signal-handler (signal)
  (when *signal-to-function*
    (remf *signal-to-function* signal)))

(defun get-signal-handler (signal)
  (getf *signal-to-function* signal))

(defun call-lisp-symbol-handler (signal-as-fixnum)
  (let ((function (get-signal-handler signal-as-fixnum)))
    (if function
        (funcall function signal-as-fixnum)
        (error 'ext:unix-signal-received :code signal-as-fixnum))))


