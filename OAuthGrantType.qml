pragma Singleton
import QtQuick 2.0

QtObject {
    readonly property string authorizationCode: "authorization_code"
    readonly property string implicit: "implicit"
    readonly property string refreshToken: "refresh_token"
    readonly property string password: "password"
    readonly property string clientCredentials: "client_credentials"
}
