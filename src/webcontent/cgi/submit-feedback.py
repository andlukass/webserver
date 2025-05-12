#!/usr/bin/env python3
import cgi
import html
import time

print("Content-Type: text/html\r\n\r\n")

form = cgi.FieldStorage()
message = form.getvalue("message", "").strip()

# Save the message into a .txt file
timestamp = str(int(time.time()))
filename = f"/tmp/{timestamp}.txt"  # Or another writable directory

try:
    with open(filename, "w") as f:
        f.write(message)
    save_status = f"Message saved to: {filename}"
except Exception as e:
    save_status = f"Error saving message: {html.escape(str(e))}"

# HTML Response
print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Feedback Submitted</title>
  <style>
    body {{
      font-family: system-ui, sans-serif;
      background-color: #ffffff;
      color: #000000;
    }}
    main {{
      padding: 5rem;
      text-align: center;
    }}
    h1 {{
      font-size: 2.5rem;
      margin-bottom: 1rem;
    }}
    blockquote {{
      font-style: italic;
      color: #444;
      margin: 1rem 0;
    }}
    .info {{
      margin-top: 1rem;
      color: #555;
    }}
    a.button {{
      margin-top: 2rem;
      display: inline-block;
      padding: 0.8rem 1.5rem;
      background-color: #000;
      color: white;
      text-decoration: none;
      border-radius: 5px;
    }}
  </style>
</head>
<body>
  <main>
    <h1>Thank You!</h1>
    <p>Your message has been submitted:</p>
    <blockquote>{html.escape(message)}</blockquote>
    <div class="info">{html.escape(save_status)}</div>
    <a href="/feedback" class="button">Return to Feedback</a>
  </main>
</body>
</html>
""")