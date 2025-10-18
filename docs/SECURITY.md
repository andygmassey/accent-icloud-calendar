# Security

Secrets
- Never commit secrets. Use environment variables (.env not committed).
- Required secrets (server): ICLOUD_USERNAME, ICLOUD_APP_PASSWORD.

Practices
- Provide .env.example only.
- Run a secret scan before publishing (gitleaks/trufflehog).
- Avoid logging credentials; redact error messages.
