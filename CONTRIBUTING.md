# Contributing

Thanks for your interest in contributing!

Development setup
- Server: Python 3.11+, `pip install -r server/requirements.txt`
- Client: PlatformIO (`pipx install platformio`), build `pio run -d client -e esp32-1248`

Workflow
- Fork and create a feature branch
- Add tests where practical (server: Flask test client; client: build checks)
- Run CI locally if possible; ensure no secrets are committed
- Open a pull request with a clear description and screenshots if UI/output changes

Coding standards
- Python: format with black/ruff (to be added), keep functions small and testable
- C++ (Arduino): keep headers lean, prefer explicit constants, avoid dynamic allocation on hot paths

Security
- Do not include any credentials in code, examples, or screenshots
- Use `.env.example` placeholders only

License
- Contributions are licensed under the repository’s license (MIT)
