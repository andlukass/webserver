#!/usr/bin/env python3
import cgi
import os
import html

print("Content-Type: text/html\r\n\r\n")

form = cgi.FieldStorage()

name = form.getvalue("name", "Anonymous")
message = form.getvalue("message", "")
file_item = form["attachment"] if "attachment" in form else None

# Save uploaded file if it exists
upload_info = ""
if file_item and file_item.filename:
    filename = os.path.basename(file_item.filename)
    filepath = f"/tmp/{filename}"
    with open(filepath, "wb") as f:
        f.write(file_item.file.read())
    upload_info = f"<p>File <strong>{html.escape(filename)}</strong> uploaded successfully.</p>"
else:
    upload_info = "<p>No file uploaded.</p>"

print(f"""
<html>
<head><title>Feedback Received</title></head>
<body>
    <h1>Thank You, {html.escape(name)}!</h1>
    <p>Your message:</p>
    <blockquote>{html.escape(message)}</blockquote>
    {upload_info}
</body>
</html>
""")
