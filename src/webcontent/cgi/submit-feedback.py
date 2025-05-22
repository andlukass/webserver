#!/usr/bin/env python3

import html
import sys
import urllib.parse

data = sys.stdin.read()
params = urllib.parse.parse_qs(data)

message = params.get("message", [""])[0]

# Save uploaded file if it exists
upload_info = ""

print("Content-Type: text/html\r\n\r\n")

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
	.message-box {{
	display: inline-block; /* shrink-wraps the content */
	background-color: #f9f9f9;
	border: 2px solid #ccc;
	border-radius: 8px;
	padding: 1rem 1.5rem;
	font-size: 1.1rem;
	font-style: italic;
	color: #333;
	margin: 0.5rem 1rem;
	box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
	}}
  </style>
</head>
<body>
  <main>
    <h1>Thank You!</h1>
    <p>Your message has been submitted:</p>
    <p><div class="message-box">{html.escape(message)}</div></p>
    {upload_info}
    <a href="/feedback" class="button">Return to Feedback</a>
  </main>
</body>
</html>
""")