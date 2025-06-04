import subprocess
import sys
import os

text = ''
SHNAME1 = 'cuts_table.tex'
SHFILE1='\documentclass{article}
\usepackage[utf8]{inputenc}

title{test}
\author{hyockey }
\date{January 2022}

\begin{document}

\maketitle

\section{Introduction}

\end{document}
'


open(SHNAME1, 'wt').write(SHFILE1)
