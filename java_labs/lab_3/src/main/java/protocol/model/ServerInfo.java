package protocol.model;

import java.util.List;

public class ServerInfo {
    private String serverName;
    private List<String> users;
    private int maxClientsNumber;


    public String getServerName() {
        return serverName;
    }

    public void setServerName(String serverName) {
        this.serverName = serverName;
    }

    public List<String> getUsers() {
        return users;
    }

    public void setUsers(List<String> users) {
        this.users = users;
    }

    public int getMaxClientsNumber() {
        return maxClientsNumber;
    }

    public void setMaxClientsNumber(int maxClientsNumber) {
        this.maxClientsNumber = maxClientsNumber;
    }
}
