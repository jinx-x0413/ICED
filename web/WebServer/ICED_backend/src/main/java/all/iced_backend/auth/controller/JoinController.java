package all.iced_backend.auth.controller;

import all.iced_backend.auth.dto.JoinDTO;
import all.iced_backend.auth.service.JoinService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
@ResponseBody
public class JoinController {

    private final JoinService joinService;

    public JoinController(JoinService joinService) {

        this.joinService = joinService;
    }

    @PostMapping("/join")
    public String joinProcess(JoinDTO joinDTO) {

        System.out.println(joinDTO.getUserid());
        // joinService.joinProcess()가 "회원가입 성공" 또는 "id가 이미 존재합니다."를 반환
        String result = joinService.joinProcess(joinDTO);

        return result; // 이미 반환 메시지가 처리되므로 여기서 결과를 반환합니다.
    }

}
