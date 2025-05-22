#!/bin/bash

read -n "$CONTENT_LENGTH" QUERY_STRING

MESSAGE=$(echo "$QUERY_STRING" | sed -n 's/^.*message=\([^&]*\).*$/\1/p' | sed 's/+/ /g' | sed 's/%20/ /g')

MESSAGE_ESCAPED=$(echo "$MESSAGE" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g')

echo "Content-Type: text/html"
echo ""

cat <<EOF
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>Feedback Submitted</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }

    body {
      font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
      background-color: #ffffff;
      color: #000000;
    }

    main {
      padding: 10rem;
      text-align: center;
    }

    h1 {
      font-size: 2.5rem;
      margin-bottom: 1rem;
    }

    p {
      font-size: 1.1rem;
      margin-bottom: 1rem;
    }

    .button {
      font-size: 1rem;
      padding: 0.8rem 1.5rem;
      background-color: #000000;
      color: white;
      border: none;
      border-radius: 5px;
      font-weight: bold;
      cursor: pointer;
      text-decoration: none;
      transition: background-color 0.3s ease;
    }

    .button:hover {
      background-color: #333333;
    }

	.message-box {
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
	}
  </style>
</head>
<body>

  <main>
    <h1>Thank You!</h1>
    <p>Your message has been submitted:</p>
	<p><div class="message-box">
	$MESSAGE_ESCAPED
	</div></p>
    <a href="/feedback_sh" class="button">Return to Feedback</a>
  </main>
</body>
</html>
EOF
