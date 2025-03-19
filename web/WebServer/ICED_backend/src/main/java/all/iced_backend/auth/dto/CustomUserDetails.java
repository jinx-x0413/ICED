package all.iced_backend.auth.dto;

import all.iced_backend.auth.entity.Users;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;

import java.util.ArrayList;
import java.util.Collection;

public class CustomUserDetails implements UserDetails {

    private final Users users;

    public CustomUserDetails(Users users) {

        this.users = users;
    }


    @Override
    public Collection<? extends GrantedAuthority> getAuthorities() {

        Collection<GrantedAuthority> collection = new ArrayList<>();

        collection.add(new GrantedAuthority() {

            @Override
            public String getAuthority() {

                return users.getRole();
            }
        });

        return collection;
    }

    @Override
    public String getPassword() {

        return users.getPassword();
    }

    @Override
    public String getUsername() {

        return users.getUserid();
    }

    public String getUserid() {
        return users.getUserid();
    }

    @Override
    public boolean isAccountNonExpired() {

        return true;
    }

    @Override
    public boolean isAccountNonLocked() {

        return true;
    }

    @Override
    public boolean isCredentialsNonExpired() {

        return true;
    }

    @Override
    public boolean isEnabled() {

        return true;
    }
}
