;; -*- lexical-binding: t; -*-

(TeX-add-style-hook
 "orthogoval_notes"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("article" "a4paper" "11pt")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("inputenc" "utf8") ("babel" "english") ("amssymb" "") ("amsmath" "") ("amsthm" "") ("mathrsfs" "") ("geometry" "left=1.0in" "right=1.0in" "top=1.0in" "bottom=1.0in") ("fontenc" "T1") ("array" "") ("longtable" "") ("multirow" "") ("calc" "") ("enumitem" "inline" "shortlabels") ("changepage" "") ("booktabs" "") ("capt-of" "") ("subcaption" "") ("sidecap" "leftcaption") ("natbib" "numbers") ("times" "") ("titlesec" "") ("xcolor" "") ("lineno" "") ("xpatch" "")))
   (TeX-run-style-hooks
    "latex2e"
    "article"
    "art11"
    "inputenc"
    "babel"
    "amssymb"
    "amsmath"
    "amsthm"
    "mathrsfs"
    "geometry"
    "fontenc"
    "array"
    "longtable"
    "multirow"
    "calc"
    "enumitem"
    "changepage"
    "booktabs"
    "capt-of"
    "subcaption"
    "sidecap"
    "natbib"
    "times"
    "titlesec"
    "xcolor"
    "lineno"
    "xpatch")
   (TeX-add-symbols
    '("bibnumfmt" 1)
    '("citenumfont" 1)
    '("smat" 1)
    '("gencite" 1)
    '("abs" 1)
    '("mmod" 2)
    '("pmod" 1)
    '("purple" 1)
    '("blue" 1)
    '("red" 1)
    "ccite"
    "ccirc"
    "ddet"
    "C"
    "Z"
    "LL"
    "z"
    "y"
    "gf"
    "gr"
    "V"
    "re"
    "imag"
    "bb"
    "Box")
   (LaTeX-add-environments
    '("proof" LaTeX-env-args ["argument"] 0)
    "theorem"
    "fact"
    "proposition"
    "lemma"
    "corollary"
    "question"
    "example"
    "definition"
    "conjecture"
    "result"
    "remark"
    "observation"
    "myabstract"
    "mykeywords"))
 :latex)

