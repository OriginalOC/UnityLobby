# Security Policy

## Supported Versions
UnityLobby is early. Security fixes will be applied to the `main` branch first, and backported only when we start tagging stable releases.

## Reporting a Vulnerability
Please report security issues privately.

**Preferred:** GitHub Security Advisories  
- Go to the repo → Security → Advisories → “Report a vulnerability”

**Alternate:** Email  
- If you don’t want to use GitHub advisories, open an issue asking for a contact method and we’ll respond with an address.

## What to include
Please include as much of the following as you can:
- A clear description of the issue and impact
- Steps to reproduce (proof-of-concept if possible)
- Affected component(s): protocol spec, server, tools, etc.
- Environment details (OS, version, config)
- Any suggested fix or mitigation

## Disclosure process
- We’ll acknowledge receipt as quickly as possible
- We’ll validate and triage severity
- We’ll work on a fix and coordinate a disclosure timeline
- Credit will be given if you want it

## Scope notes
UnityLobby touches:
- network parsing (UnityLink)
- file storage (object repo, saves)
- room/multiplayer relay

These areas are in-scope and should be reported privately if you find vulnerabilities.
