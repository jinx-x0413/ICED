package all.iced_backend;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableAsync;

@SpringBootApplication
@EnableAsync
public class IcedBackendApplication {

	public static void main(String[] args) {
		SpringApplication.run(IcedBackendApplication.class, args);
	}

}
