pragma Singleton
import QtQuick 2.0

OAuth2Provider {
    baseUrl: "https://accounts.google.com/o"
    authUrl: baseUrl + "/oauth2/auth"
    tokenUrl: baseUrl + "/oauth2/token"
}
