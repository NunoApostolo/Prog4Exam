#pragma once
#include "IMouseHandler.h"
#include "Camera.h"
#include "SceneManager.h"
#include "Scene.h"
#include <vector>

//IMouseHandler::IMouseHandler()
//{
//	//don't have time for this
////	std::vector<GameObject*> go = SceneManager::GetInstance().curScene->GetAllObjs();
////
////	for (auto& g : go) {
////		for (auto& comp : g->GetAllComponents()) {
////			if ((typename("")))
////		}
////	}
//}

void IMouseHandler::Init(GameObject* gameObject)
{
	tex = gameObject->GetComponent<TextureRenderer>();
	if (tex != nullptr) {
		std::vector<Event> e{ Event::MouseMove, Event::MouseDown, Event::MouseUp };
		EventManager::GetInstance().AddListener(tex->gameObject, this, e);
	}
}

void IMouseHandler::HandleEvent(Event e, EventArgs args)
{
	if (!tex->gameObject->IsActive()) return;
	if (e == Event::MouseMove) {
		if (tex != nullptr) {
			Vector2 pointerPos{};
			if (!isMouseOver) {
				if (IsPointInTex(Vector2(args[1], args[2]), pointerPos)) {
					isMouseOver = true;
					OnPointerEnter(PointerData(-1, pointerPos));
				}
			}
			else {
				if (!(IsPointInTex(Vector2(args[1], args[2]), pointerPos))) {
					isMouseOver = false;
					OnPointerExit(PointerData(-1, pointerPos));
				}
			}
		}
	}
	if (e == Event::MouseDown) {
		if (tex != nullptr) {
			Vector2 pointerPos{};
			if (IsPointInTex(Vector2(args[1], args[2]), pointerPos)) {
				OnPointerDown(PointerData(args[0], pointerPos));
			}
		}
	}
	if (e == Event::MouseUp) {
		if (tex != nullptr) {
			Vector2 pointerPos{};
			if (IsPointInTex(Vector2(args[1], args[2]), pointerPos)) {
				OnPointerUp(PointerData(args[0], pointerPos));
			}
		}
	}
}

bool IMouseHandler::IsPointInTex(Vector2 args, Vector2& pointerPos)
{
	pointerPos = Vector2(static_cast<float>(args.x - Camera::GetMainCamera().GetViewPort().width / 2),
		static_cast<float>(args.y - Camera::GetMainCamera().GetViewPort().height / 2));
	Vector2 pos = Vector2(tex->gameObject->transform->GetPosition().x - tex->GetScaledSize().x * tex->GetPivot().x,
		tex->gameObject->transform->GetPosition().y - tex->GetScaledSize().y * tex->GetPivot().y);
	Vector2 bounds = Vector2((tex->GetScaledSize().x + pos.x),
		(tex->GetScaledSize().y + pos.y));
	return (pointerPos.x >= pos.x && pointerPos.y >= pos.y && pointerPos.x <= bounds.x && pointerPos.y <= bounds.y);
}
