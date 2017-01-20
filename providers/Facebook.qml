pragma Singleton
import QtQuick 2.0

OAuth2Provider {
    authUrl: "https://www.facebook.com/dialog/oauth"
    tokenUrl: "https://graph.facebook.com/oauth/access_token"
    accessTokenMethod: "GET"
}
