{
	"service": {
		"api": "fastcgi",
		"port": 3000
	},
	"session": {
		"location": "client",
		"expire": "renew",
		"timeout": 3600,
		"client": {
			"hmac": "sha1",
			"hmac_key": "8e5e93c4f1e5ff719a0ef3a35af3cd55"
		}
	},
	"http": {
		"script_names": ["/"]
	}
}