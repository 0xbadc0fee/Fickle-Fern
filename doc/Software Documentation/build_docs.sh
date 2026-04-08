#!/bin/bash

# Target directory
TARGET_DIR="output_HTML"
DOXYFILE="Doxyfile_HTML"
HTML_DIR="output_HTML/html/index.html"
HTML_CHM="output_CHM/html"
TARGET_DIR="output_HTML"

#Run Doxygen HTML
#Need Graphviz installed https://graphviz.org/download/
echo "Running HTML Doxygen..."
doxygen "$DOXYFILE" || { echo "❌ Doxygen HTML failed."; exit 1; }
#Run Doxygen CHM
#echo "Running CHM Doxygen..."
#Need old??
#doxygen "$DOXYFILE_CHM" || { echo "❌ Doxygen CHM failed."; exit 1; }

# Create the redirect file
cat  <<EOF > "$TARGET_DIR/index.html"
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv="refresh" content="0; URL='html/index.html'" />
  </head>
  <body>
    <p>If you are not redirected automatically, <a href='html/index.html'>documentation</a>click here</p>
  </body>
</html>
EOF

echo "✅ Redirect index.html created in $TARGET_DIR"

