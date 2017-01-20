pragma Singleton
import QtQuick 2.0

OAuth2Provider {
    baseUrl: "https://login.live.com"
    authUrl: baseUrl + "/oauth20_authorize.srf"
    tokenUrl: baseUrl + "/oauth20_token.srf"
}
