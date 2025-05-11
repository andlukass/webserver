#!/usr/bin/env python3
import cgi
import html

print("Content-Type: text/html\r\n\r\n") #I'm not sure if it's needed

try:
    form = cgi.FieldStorage()
    message = form.getvalue("message", "")
except Exception as e:
    print("Content-Type: text/html\r\n\r\n")
    print(f"<h1>CGI Script Error</h1><p>{html.escape(str(e))}</p>")
    exit(1)

# Save uploaded file if it exists
upload_info = ""


print(f"""
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Feedback Submitted</title>
  <style>
    body {{
      font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
      background-color: #ffffff;
      color: #000000;
    }}
    main {{
      padding: 10rem;
      text-align: center;
    }}
    h1 {{
      font-size: 3rem;
      margin-bottom: 1rem;
    }}
    p {{
      font-size: 1.2rem;
      margin-bottom: 2rem;
    }}
    a.button {{
      display: inline-block;
      padding: 0.8rem 1.5rem;
      background-color: #000000;
      color: white;
      text-decoration: none;
      border-radius: 5px;
      font-weight: bold;
      transition: background-color 0.3s ease;
    }}
    a.button:hover {{
      background-color: #333333;
    }}
  </style>
</head>
<body>
  <main>
    <h1>Thank You!</h1>
    <p>Your message has been submitted:</p>
    <blockquote>{html.escape(message)}</blockquote>
    {upload_info}
    <a href="/feedback" class="button">Return to Feedback</a>
  </main>
</body>
</html>
""")