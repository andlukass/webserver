#!/bin/bash

# Чтение тела POST-запроса
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
    body { font-family: sans-serif; background: #f9f9f9; color: #222; }
    main { padding: 10rem; text-align: center; }
    .button { padding: 0.8rem 1.5rem; background: black; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <main>
    <h1>Thank You!</h1>
    <p>Your message has been submitted:</p>
    <blockquote>$MESSAGE_ESCAPED</blockquote>
    <a href="/feedback" class="button">Return to Feedback</a>
  </main>
</body>
</html>
EOF
